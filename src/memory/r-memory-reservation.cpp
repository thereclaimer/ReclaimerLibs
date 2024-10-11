#pragma once

#include "r-memory-internal.hpp"

/**********************************************************************************/
/* EXTERNAL                                                                       */
/**********************************************************************************/

r_external const RMemoryReservationHandle 
r_mem::reserve(
    const r_cstr reservation_tag,
    const r_size minimum_reservation_size) {

    //get our sizes
    const r_size reservation_alignment    = r_mem::memory_manager_reservation_alignment(); 
    const r_size reservation_size_aligned = r_align_a_to_b(minimum_reservation_size,reservation_alignment);

    //make sure we can add this reservation
    r_b8 can_add_reservation = r_mem_internal::reservation_list_can_add(reservation_size_aligned);
    if (!can_add_reservation) {
        return(NULL);
    }

    //make the system memory reservation
    r_memory reservation_start =
        r_mem_internal::platform_memory_reserve(
            NULL,
            reservation_size_aligned);
    if (!reservation_start) {
        return(NULL);
    }

    //push a new reservation structure on the stack
    RMemoryReservation* reservation_ptr = r_mem_internal::stack_push_reservation();
    if (!reservation_ptr) {
        return(NULL);
    }

    //initialize the reservation
    reservation_ptr->next                 = NULL;
    reservation_ptr->region_list          = {0};
    reservation_ptr->block_allocator_list = {0};
    reservation_ptr->stack_allocator_list = {0};
    reservation_ptr->start                = reservation_start;
    reservation_ptr->size                 = reservation_size_aligned;
    reservation_ptr->position             = 0;
    reservation_ptr->index                = 0;

    //add the reservation to the list
    r_mem_internal::reservation_list_add(reservation_ptr);

    //return the new reservation
    return(reservation_ptr);
}

r_external const r_size 
r_mem::reservation_size_total(
    const RMemoryReservationHandle reservation_handle) {

    RMemoryReservation* reservation_ptr = r_mem_internal::reservation_from_handle(reservation_handle);
    
    return(reservation_ptr ? reservation_ptr->size : 0);
}

r_external const r_size 
r_mem::reservation_size_used(
    const RMemoryReservationHandle reservation_handle) {

    RMemoryReservation* reservation_ptr = r_mem_internal::reservation_from_handle(reservation_handle);

    if (!reservation_ptr) {
        return(0);
    }

    //TODO
    return(0);
}

r_external const r_size 
r_mem::reservation_size_free(
    const RMemoryReservationHandle reservation_handle) {

    RMemoryReservation* reservation_ptr = r_mem_internal::reservation_from_handle(reservation_handle);

    r_size size_free = reservation_ptr->size;

    for (
        RMemoryRegion* current_region = reservation_ptr->region_list.first;
        current_region != NULL;
        current_region = current_region->next) {

        size_free -= current_region->region_size;
    }

    return(size_free);
}

r_external const r_size 
r_mem::reservation_list_count() {

    RMemoryReservationList& reservation_list = r_mem_internal::memory_manager_get_reservation_list();

    return(reservation_list.count);
}

r_external const r_size 
r_mem::reservation_arena_region_count(
    const RMemoryReservationHandle reservation_handle) {

    RMemoryReservation* reservation_ptr = r_mem_internal::reservation_from_handle(reservation_handle);

    return (reservation_ptr ? reservation_ptr->region_list.count : 0);
}

r_external const RMemoryReservationHandle 
r_mem::reservation_list(r_void) {

    RMemoryReservationList& reservation_list = r_mem_internal::memory_manager_get_reservation_list();

    return(reservation_list.first);
}

r_external const RMemoryReservationHandle 
r_mem::reservation_next(
    const RMemoryReservationHandle reservation_handle) {

    RMemoryReservation* reservation_ptr = r_mem_internal::reservation_from_handle(reservation_handle);

    return (reservation_ptr ? reservation_ptr->next : NULL);
}

r_external const RMemoryReservationHandle 
r_mem::reservation_at_index(
    const r_index reservation_index) {
    
    RMemoryReservationList& reservation_list = r_mem_internal::memory_manager_get_reservation_list();
    RMemoryReservation*     reservation_ptr  = reservation_list.first; 

    //sanity check
    if (reservation_list.count == 0 ||
        reservation_index >= reservation_list.count) {
        
        return(NULL);
    }

    RMemoryReservation* selected_reservation = NULL;
    RMemoryReservation* current_reservation  = reservation_list.first;

    //go through the reservation list
    for (
        r_index current_index = 0;
        current_index < reservation_list.count;
        ++current_index) {

        //if we con't have a reservation at this index, we're done
        if (!current_reservation) {
            break;
        }

        //we found the index
        if (current_index == reservation_index) {

            //set the selected reservation if the indexes match
            if (current_reservation->index == reservation_index) {
                selected_reservation = current_reservation;
            }

            break;
        }

        //get the next reservation
        current_reservation = current_reservation->next;
    }

    //return whatever we found
    return(selected_reservation);
}

/**********************************************************************************/
/* INTERNAL                                                                       */
/**********************************************************************************/


r_internal r_void 
r_mem_internal::reservation_list_add(
    RMemoryReservation* reservation_ptr) {

    if (!reservation_ptr) {
        return;
    }
 
    RMemoryReservationList& reservation_list = r_mem_internal::memory_manager_get_reservation_list();

    reservation_ptr->index = reservation_list.count;

    if (!reservation_list.first) {
        reservation_list.first = reservation_ptr;
        reservation_list.last  = reservation_ptr;
    }

    reservation_list.last->next           = reservation_ptr;
    reservation_list.last                 = reservation_ptr;
    reservation_list.total_reserved_size += reservation_ptr->size;

    ++reservation_list.count;
}

r_internal const r_b8
r_mem_internal::reservation_list_release_all(
    r_void) {

    RMemoryReservationList& reservation_list = r_mem_internal::memory_manager_get_reservation_list();

    if (reservation_list.count == 0) {
        return(true);
    }

    r_b8 released_all_reservations = true;

    for (
        RMemoryReservation* reservation_ptr = reservation_list.first;
        reservation_ptr != NULL;
        reservation_ptr = reservation_ptr->next) {

        released_all_reservations &= 
            r_mem_internal::platform_memory_release(
                reservation_ptr->start,
                reservation_ptr->size);
    }

    return(released_all_reservations);
}

r_internal const r_b8
r_mem_internal::reservation_list_can_add(
    const r_size reservation_size) {

    RMemoryReservationList& reservation_list = r_mem_internal::memory_manager_get_reservation_list();

    const r_size new_list_size     = reservation_list.total_reserved_size + reservation_size;
    const r_size max_reserved_size = r_mem::memory_manager_maximum_reserved_size();

    return(new_list_size <= max_reserved_size);
}