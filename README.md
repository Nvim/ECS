# ECS

This is a small library intended for my own use in game engine and graphics programming projects.

I didn't go with a pre-made library such as [EnTT](https://github.com/skypjack/entt) or [Flecs](https://github.com/SanderMertens/flecs) 
because they provide a lot more features that I need for my current purposes, although they were a great inspiration for designing
my API.

## Implementation details

The ECS uses Sparse Sets internally to keep track of which entities have each component, called 'pools'. There is a pool instance per
component type, and they are all lazy-loaded dynamically. No need to declare which components you want the ECS to work with, nor
to specify a component count.

To map each component type to its pool, I use `std::type_hash` which uses the compiler's internal ID of the object's type to 
provide a hash code. This hash code is unique to all types, even template specializations.

Entity IDs are 32 bits unsigned integers. All bits are dedicated to the ID for now, and there's currently no ID recycling system.

## TODO

- [ ] Add real unit tests
- [ ] Better API for queries (return an iterable `View` type)
- [ ] ID recycling
- [ ] Refactor `ecs.hpp` to clean up `typeid` boilerplate
- [ ] More configurability through defines, or a config struct to pass on init
- [ ] Try to find cool usages for bitmasks on the IDS
- [ ] Better demo program
