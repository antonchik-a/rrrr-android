/* router.h */

#ifndef _ROUTER_H
#define _ROUTER_H

#include "config.h"
#include "router.h"

#include "rrrr_types.h"
#include "tdata.h"
#include "bitset.h"
#include "hashgrid.h"

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

/* When associated with a stop index,
 * a router_state_t describes a leg of an itinerary.
 */
typedef struct router_state router_state_t;
/* We could potentially remove the back_time from router_state,
 * but this requires implementing some lookup functions and storing
 * the back_trip_stop rather than the back_stop (global stop index):
 * a trip can pass through a stop more than once.
 */
/* TODO rename members to ride_from, walk_from, route, trip, */
struct router_state {
    /* The index of the previous stop in the itinerary */
    uint32_t ride_from;
    /* The index of the route used to travel from back_stop to here, or WALK  */
    uint32_t back_route;
    /* The index of the trip used to travel from back_stop */
    uint32_t back_trip;
    /* The time when this stop was reached */
    rtime_t  time;
    /* The time at which the trip within back_route left back_stop */
    rtime_t  board_time;

    /* Second phase footpath/transfer results */
    /* The stop from which this stop was reached by walking (2nd phase) */
    uint32_t walk_from;

    /* The time when this stop was reached by walking (2nd phase) */
    rtime_t  walk_time;

    uint16_t back_route_stop;
    uint16_t route_stop;
};

/* Scratch space for use by the routing algorithm.
 * Making this opaque requires more dynamic allocation.
 */
typedef struct router router_t;
struct router {
    /* The transit / timetable data tables */
    tdata_t *tdata;

    /* The best known time at each stop */
    rtime_t *best_time;

    /* One router_state_t per stop, per round */
    router_state_t *states;

    /* Used to track which stops improved during each round */
    BitSet *updated_stops;

    /* Used to track which routes might have changed during each round */
    BitSet *updated_routes;

#ifdef RRRR_BANNED_ROUTES_BITMASK
    /* Used to bann routes and in the final clockwise search optimise */
    BitSet *banned_routes;
#endif

    uint32_t origin;
    uint32_t target;

    calendar_t day_mask;
    serviceday_t servicedays[3];

#ifdef RRRR_FEATURE_LATLON
    HashGrid hg;
#endif
    /* TODO: We should move more routing state in here,
     * like round and sub-scratch pointers.
     */
};

struct route_cache {
    route_t *this_route;
    uint32_t *route_stops;
    uint8_t  *route_stop_attributes;
    trip_t   *route_trips;
    calendar_t *trip_masks;
    uint32_t route_index;
    bool route_overlap;
};

typedef struct route_cache route_cache_t;


/* FUNCTION PROTOTYPES */

bool router_setup(router_t*, tdata_t*);

void router_reset(router_t *router);

void router_teardown(router_t*);

bool router_route(router_t*, router_request_t*);

void router_round(router_t *router, router_request_t *req, uint8_t round);

#endif /* _ROUTER_H */

