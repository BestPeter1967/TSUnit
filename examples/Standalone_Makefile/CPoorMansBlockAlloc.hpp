/* ==========================================================================
 * @(#)File: CPoorMansBlockAlloc.hpp
 * Created: 2022-02-01
 * --------------------------------------------------------------------------
 *  (c)1982-2022 Tangerine-Software
 *
 *       Hans-Peter Beständig
 *       Kühbachstr. 8
 *       81543 München
 *       GERMANY
 *
 *       mailto:hdusel@tangerine-soft.de
 *       http://hdusel.tangerine-soft.de
 * --------------------------------------------------------------------------
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * ========================================================================== */
#pragma once
#include <cstdlib>

template <typename T, size_t NR_OF_ELEMENTS_LOG2N>
class CPoorMansBlockAlloc
{
    T _buffer[1<<NR_OF_ELEMENTS_LOG2N];
    uint8_t _usedFlags = 0b00000000;
    uint8_t _usedCount = 0;

public:
    CPoorMansBlockAlloc() = default;

    /*!
     * \brief Asks how many storage slots of the allocators type \p T are currently in use.
     * \return The number of the used slots of the type \p T.
     * \see full() const
     * \see empty() const
     * \see available() const
     * \see used() const
     */
    size_t used() const {
        return _usedCount;
    }

    /*!
     * \brief Asks how many storage slots of the allocators type \p T are currently still available.
     * \return The number of the available slots of the type \p T.
     * \see full() const
     * \see empty() const
     * \see available() const
     * \see used() const
     */
    size_t available() const {
        return _totalSize() - _usedCount;
    }

    /*!
     * \brief Asks if the Allocator is currently empty.
     * \return true if no blocks are currently allocated, false otherwise.
     * \see full() const
     * \see empty() const
     * \see available() const
     * \see used() const
     */
    bool empty() const
    {
        return 0 == used();
    }

    /*!
     * \brief Asks if the Allocator is completely occupied.
     * \return true if no blocks are currently available, false otherwise.
     * \see empty() const
     * \see available() const
     * \see used() const
     */
    bool full() const
    {
        return 0 == available();
    }

    /*!
     * \brief Attempt to allocate a new slot of the type \p T and return its storge pointer.
     * \return Upon success a pointer to an storage of an element of the type \p T.
     * If the allocation fails (because the allocator is full) then a nullptr is returned.
     * \see free(T*)
     */
    T* alloc()
    {
        return full() ? nullptr : _allocateNextFreeIndex();
    }

    /*!
     * \brief Frees an pointer to a slot of the type \p T that has been allocated previously by alloc().
     * \note Users of these pointers should never rely on these contents afterwards.
     * \param inBuffPtr A Pointer that has been prev. allocated by \ref alloc().
     * \return true upon success. false on failure (e.g. already freed or if \p is inBuffPtr not an owner of this allocator)
     * \see alloc(T*)
     */
    bool free(T* inBuffPtr)
    {
        bool success = false;
        if (nullptr == inBuffPtr)
        {
            success = true;
        }
        else
        {
            if (not empty())
            {
                size_t offset = inBuffPtr - _buffer;
                if (offset < _totalSize())
                {
                    const uint32_t mask(1<<offset);
                    if (_usedFlags & mask) // Just release slots that are actually used
                    {
                        _usedFlags &= ~mask;
                        --_usedCount;
                        success = true;
                    }
                    else
                    {
                        success = false;
                    }
                }
                else
                {
                    success = false;
                }
            }
            else
            {
                success = false;
            }
        }
        return success;
    }

    /*!
     * \brief Clears all allocations within this object. This call effectively
     * resets all allocation buffers.
     * \note Users of these pointers should never rely on these contents afterwards.
     */
    void clear()
    {
        _usedCount = 0;
        _usedFlags = 0b00000000;
    }

private:

    constexpr size_t _mask()      const { return (1<<NR_OF_ELEMENTS_LOG2N) -1;}
    constexpr size_t _totalSize() const { return (1<<NR_OF_ELEMENTS_LOG2N);}

    static constexpr uint8_t kIndexNotFound = 0xff;
    T* _allocateNextFreeIndex()
    {
        T* foundSlot = nullptr;
        for (unsigned int idx = 0; idx < _totalSize(); ++idx)
        {
            const uint32_t mask(1 << idx);
            if (not (_usedFlags & mask))
            {
                _usedFlags |= mask;
                foundSlot = &_buffer[idx];
                ++_usedCount;
                break;
            }
        }
        return foundSlot;
    }

};
