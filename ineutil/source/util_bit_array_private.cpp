/*-*-c++-*-*************************************************************************************************************
* Copyright 2016 - 2022 Inesonic, LLC.
* 
* This file is licensed under two licenses.
*
* Inesonic Commercial License, Version 1:
*   All rights reserved.  Inesonic, LLC retains all rights to this software, including the right to relicense the
*   software in source or binary formats under different terms.  Unauthorized use under the terms of this license is
*   strictly prohibited.
*
* GNU Public License, Version 2:
*   This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
*   License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later
*   version.
*   
*   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
*   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
*   details.
*   
*   You should have received a copy of the GNU General Public License along with this program; if not, write to the Free
*   Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
********************************************************************************************************************//**
* \file
*
* This file implements the \ref Util::BitArray class.
***********************************************************************************************************************/

#include <QString>
#include <QHash>
#include <QList>

#include <cstdint>
#include <cstring>
#include <cassert>
#include <algorithm>

#include "util_bit_functions.h"
#include "util_bit_array.h"
#include "util_bit_array_private.h"

namespace Util {
    BitArray::Private::Private() {
        data       = nullptr;
        dataLength = 0;
        bitLength  = 0;
    }


    BitArray::Private::Private(BitArray::Index numberBits, bool value) {
        bitLength  = numberBits;
        dataLength = allocationDataSize(numberBits);
        data       = new AllocationUnit[dataLength];

        if (value == false) {
            memset(reinterpret_cast<std::uint8_t*>(data), 0, dataLength * allocationUnitSize / 8);
        } else {
            unsigned long completeAllocationUnits = numberBits / allocationUnitSize;
            unsigned      residue                 = numberBits % allocationUnitSize;

            memset(reinterpret_cast<std::uint8_t*>(data), 0xFF, completeAllocationUnits * allocationUnitSize / 8);
            if (residue > 0) {
                AllocationUnit lastAllocationUnit = (static_cast<AllocationUnit>(1) << residue) - 1;
                data[dataLength - 1] = lastAllocationUnit;
            }
        }
    }


    BitArray::Private::Private(const bool* rawData, BitArray::Index numberBits) {
        if (numberBits == 0) {
            data       = nullptr;
            dataLength = 0;
            bitLength  = 0;
        } else {
            bitLength  = numberBits;
            dataLength = allocationDataSize(numberBits);
            data       = new AllocationUnit[dataLength];

            AllocationUnit* dataPointer = data;
            AllocationUnit  unit        = 0;
            AllocationUnit  mask        = 1;
            for (BitArray::Index index=0 ; index<numberBits ; ++index) {
                bool value = rawData[index];

                if (value) {
                    unit |= mask;
                } else {
                    unit &= ~mask;
                }

                mask <<= 1;
                if (mask == 0) {
                    *dataPointer = unit;
                    ++dataPointer;

                    unit = 0;
                    mask = 1;
                }
            }

            if (mask != 1) {
                *dataPointer = unit;
            }
        }
    }


    BitArray::Private::Private(const void* rawData, BitArray::Index numberBits) {
        if (numberBits == 0) {
            data       = nullptr;
            dataLength = 0;
            bitLength  = 0;
        } else {
            bitLength  = numberBits;
            dataLength = allocationDataSize(numberBits);
            data       = new AllocationUnit[dataLength];

            unsigned long numberBytes           = (numberBits + 7) / 8;
            unsigned long allocationSizeInBytes = dataLength * (allocationUnitSize / 8);
            unsigned      residue               = allocationSizeInBytes - numberBytes;

            memcpy(data, rawData, numberBytes);
            if (residue > 0) {
                memset(reinterpret_cast<std::uint8_t*>(data) + numberBytes, 0, residue);
            }
        }
    }


    BitArray::Private::Private(const BitArray::Private& other):QSharedData(other) {
        if (other.data != nullptr) {
            data = new AllocationUnit[other.dataLength];
            memcpy(data, other.data, other.dataLength * (allocationUnitSize / 8));
        } else {
            data = nullptr;
        }

        dataLength = other.dataLength;
        bitLength  = other.bitLength;
    }


    BitArray::Private::~Private() {
        if (data != nullptr) {
            delete[] data;
        }
    }


    BitArray::Index BitArray::Private::size() const {
        return bitLength;
    }


    void BitArray::Private::clear() {
        if (data != nullptr) {
            delete[] data;
        }

        data       = nullptr;
        dataLength = 0;
        bitLength  = 0;
    }


    void BitArray::Private::resize(Index newLength) {
        if (newLength != bitLength) {
            if (newLength == 0) {
                if (data != nullptr) {
                    delete[] data;
                }

                data       = nullptr;
                dataLength = 0;
                bitLength  = 0;
            } else if (data == nullptr) {
                unsigned long   newDataLength = allocationDataSize(newLength);
                AllocationUnit* newData       = new AllocationUnit[newDataLength];
                memset(newData, 0, newDataLength * (allocationUnitSize / 8));

                data       = newData;
                dataLength = newDataLength;
                bitLength  = newLength;
            } else {
                unsigned long   newDataLength = allocationDataSize(newLength);
                AllocationUnit* newData       = new AllocationUnit[newDataLength];

                unsigned long   oldDataLength = dataLength;
                AllocationUnit* oldData       = data;

                if (newDataLength > oldDataLength) {
                    memcpy(newData, oldData, oldDataLength * (allocationUnitSize / 8));
                    memset(newData + oldDataLength, 0, (newDataLength - oldDataLength) * (allocationUnitSize / 8));
                } else /* if (newDataLength <= oldDataLength) */ {
                    memcpy(newData, oldData, newDataLength * (allocationUnitSize / 8));
                }

                data       = newData;
                dataLength = newDataLength;
                bitLength  = newLength;

                delete[] oldData;
            }
        }
    }


    void BitArray::Private::setBit(BitArray::Index bitIndex, bool nowSet) {
        resizeToFit(bitIndex);

        unsigned long  unitIndex = bitIndex / allocationUnitSize;
        unsigned       bitOffset = bitIndex % allocationUnitSize;
        AllocationUnit mask      = static_cast<AllocationUnit>(1) << bitOffset;
        AllocationUnit unit      = data[unitIndex];

        if (nowSet) {
            unit |= mask;
        } else {
            unit &= ~mask;
        }

        data[unitIndex] = unit;
    }


    void BitArray::Private::clearBit(BitArray::Index bitIndex, bool nowCleared) {
        setBit(bitIndex, !nowCleared);
    }


    void BitArray::Private::setBits(BitArray::Index startingIndex, BitArray::Index endingIndex, bool nowSet) {
        assert(startingIndex <= endingIndex);

        resizeToFit(endingIndex);

        unsigned long startingUnitIndex = startingIndex / allocationUnitSize;
        unsigned      startingBitOffset = startingIndex % allocationUnitSize;

        unsigned long endingUnitIndex   = endingIndex / allocationUnitSize;
        unsigned      endingBitOffset   = endingIndex % allocationUnitSize;

        if (startingUnitIndex == endingUnitIndex) {
            AllocationUnit unit      = data[startingUnitIndex];
            AllocationUnit startMask = static_cast<AllocationUnit>(-1) << startingBitOffset;
            AllocationUnit endMask   =   endingBitOffset == (allocationUnitSize - 1)
                                       ? static_cast<AllocationUnit>(-1)
                                       : ~(static_cast<AllocationUnit>(-1) << (endingBitOffset + 1));
            AllocationUnit mask      = startMask & endMask;

            if (nowSet) {
                unit |= mask;
            } else {
                unit &= ~mask;
            }

            data[startingUnitIndex] = unit;
        } else {
            unsigned long startingWholeUnitIndex;
            if (startingBitOffset == 0) {
                startingWholeUnitIndex = startingUnitIndex;
            } else {
                AllocationUnit unit      = data[startingUnitIndex];
                AllocationUnit startMask = static_cast<AllocationUnit>(-1) << startingBitOffset;

                if (nowSet) {
                    unit |= startMask;
                } else {
                    unit &= ~startMask;
                }

                data[startingUnitIndex] = unit;
                startingWholeUnitIndex = startingUnitIndex + 1;
            }

            unsigned long endingWholeUnitIndex;
            if (endingBitOffset == (allocationUnitSize - 1)) {
                endingWholeUnitIndex = endingUnitIndex;
            } else {
                AllocationUnit unit    = data[endingUnitIndex];
                AllocationUnit endMask = (static_cast<AllocationUnit>(1) << (endingBitOffset + 1)) - 1;

                if (nowSet) {
                    unit |= endMask;
                } else {
                    unit &= ~endMask;
                }

                data[endingUnitIndex] = unit;
                endingWholeUnitIndex = endingUnitIndex - 1;
            }

            if (endingWholeUnitIndex >= startingWholeUnitIndex) {
                std::uint8_t fillValue = nowSet ? static_cast<std::uint8_t>(-1) : 0;
                memset(
                    data + startingWholeUnitIndex,
                    fillValue,
                    (endingWholeUnitIndex - startingWholeUnitIndex + 1) * (allocationUnitSize / 8)
                );
            }
        }
    }


    void BitArray::Private::clearBits(BitArray::Index startingIndex, BitArray::Index endingIndex, bool nowCleared) {
        setBits(startingIndex, endingIndex, !nowCleared);
    }


    bool BitArray::Private::isSet(BitArray::Index index) const {
        bool result;

        if (index >= bitLength) {
            result = false;
        } else {
            unsigned long  unitIndex = index / allocationUnitSize;
            unsigned       bitOffset = index % allocationUnitSize;
            AllocationUnit unit      = data[unitIndex];
            AllocationUnit mask      = static_cast<AllocationUnit>(1) << bitOffset;

            result = (unit & mask) ? true : false;
        }

        return result;
    }


    bool BitArray::Private::isClear(BitArray::Index index) const {
        return !isSet(index);
    }


    BitArray::Index BitArray::Private::firstSetBit() const {
        BitArray::Index result;

        if (data != nullptr) {
            unsigned long index = 0;
            while (index < dataLength && data[index] == 0) {
                ++index;
            }

            if (index < dataLength) {
                AllocationUnit unit = data[index];
                AllocationUnit mask = maskLsbOne(unit);

                if (allocationUnitSize == 64) {
                    result = allocationUnitSize * index + msbLocation64(mask);
                } else {
                    result = allocationUnitSize * index + msbLocation32(static_cast<std::uint32_t>(mask));
                }

                if (result >= bitLength) {
                    result = BitArray::invalidIndex;
                }
            } else {
                result = BitArray::invalidIndex;
            }

        } else {
            result = BitArray::invalidIndex;
        }

        return result;
    }


    BitArray::Index BitArray::Private::firstClearedBit() const {
        BitArray::Index result;

        if (data != nullptr) {
            unsigned long index = 0;
            while (index < dataLength && data[index] == static_cast<AllocationUnit>(-1)) {
                ++index;
            }

            if (index < dataLength) {
                AllocationUnit unit = data[index];
                AllocationUnit mask = maskLsbZero(unit);

                if (allocationUnitSize == 64) {
                    result = allocationUnitSize * index + msbLocation64(mask);
                } else {
                    result = allocationUnitSize * index + msbLocation32(static_cast<std::uint32_t>(mask));
                }

                if (result >= bitLength) {
                    result = BitArray::invalidIndex;
                }
            } else {
                result = BitArray::invalidIndex;
            }

        } else {
            result = BitArray::invalidIndex;
        }

        return result;
    }


    BitArray::Index BitArray::Private::firstSetBit(BitArray::Index startingIndex) const {
        BitArray::Index result;

        if (data != nullptr && startingIndex < bitLength) {
            unsigned long  index = startingIndex / allocationUnitSize;
            AllocationUnit mask  = static_cast<AllocationUnit>(-1) << (startingIndex % allocationUnitSize);
            while (index < dataLength && (data[index] & mask) == 0) {
                mask = static_cast<AllocationUnit>(-1);
                ++index;
            }

            if (index < dataLength) {
                AllocationUnit unit    = data[index] & mask;
                AllocationUnit oneMask = maskLsbOne(unit);

                if (allocationUnitSize == 64) {
                    result = allocationUnitSize * index + msbLocation64(oneMask);
                } else {
                    result = allocationUnitSize * index + msbLocation32(static_cast<std::uint32_t>(oneMask));
                }

                if (result >= bitLength) {
                    result = BitArray::invalidIndex;
                }
            } else {
                result = BitArray::invalidIndex;
            }

        } else {
            result = BitArray::invalidIndex;
        }

        return result;
    }


    BitArray::Index BitArray::Private::firstClearedBit(BitArray::Index startingIndex) const {
        BitArray::Index result;

        if (data != nullptr && startingIndex < bitLength) {
            unsigned long  index = startingIndex / allocationUnitSize;
            AllocationUnit mask  = (static_cast<AllocationUnit>(1) << (startingIndex % allocationUnitSize)) - 1;
            while (index < dataLength && (data[index] | mask) == static_cast<AllocationUnit>(-1)) {
                mask = 0;
                ++index;
            }

            if (index < dataLength) {
                AllocationUnit unit     = data[index] | mask;
                AllocationUnit zeroMask = maskLsbZero(unit);

                if (allocationUnitSize == 64) {
                    result = allocationUnitSize * index + msbLocation64(zeroMask);
                } else {
                    result = allocationUnitSize * index + msbLocation32(static_cast<std::uint32_t>(zeroMask));
                }

                if (result >= bitLength) {
                    result = BitArray::invalidIndex;
                }
            } else {
                result = BitArray::invalidIndex;
            }

        } else {
            result = BitArray::invalidIndex;
        }

        return result;
    }


    bool BitArray::Private::operator==(const BitArray::Private& other) const {
        bool isEqual;

        if (data == nullptr) {
            if (other.data == nullptr) {
                isEqual = true;
            } else {
                isEqual = false;
            }
        } else {
            if (other.bitLength == bitLength) {
                isEqual = true;

                unsigned long index = 0;
                while (index < dataLength && isEqual) {
                    if (index < (dataLength - 1)) {
                        isEqual = (data[index] == other.data[index]);
                    } else {
                        unsigned long  baseBitIndex = bitLength - index * allocationUnitSize;
                        if (baseBitIndex == allocationUnitSize) {
                            isEqual = (data[index] == other.data[index]);
                        } else {
                            assert(baseBitIndex < allocationUnitSize);

                            AllocationUnit mask         = (static_cast<AllocationUnit>(1) << baseBitIndex) - 1;
                            AllocationUnit unit         = data[index] & mask;
                            AllocationUnit otherUnit    = other.data[index] & mask;

                            isEqual = (unit == otherUnit);
                        }
                    }

                    ++index;
                }
            } else {
                isEqual = false;
            }
        }

        return isEqual;
    }


    bool BitArray::Private::operator!=(const BitArray::Private& other) const {
        return !operator==(other);
    }


    unsigned long BitArray::Private::allocationDataSize(BitArray::Index bitLength) {
        return (bitLength + allocationUnitSize - 1) / allocationUnitSize;
    }


    void BitArray::Private::resizeToFit(BitArray::Index index) {
        if (index >= bitLength) {
            resize(index + 1);
        }
    }
}
