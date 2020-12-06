# RayTracing

This is my personal toy ray tracing c++ implementation.

I read through Ray Tracing In One Weekend seires and then make my own design decisions.

---

## Some hard to find bugs i encounter:
- When the light is refracted, move the hit point along the direction a bit so that the new starting point of the ray is in oposite side of the surface, otherwise, the ray will hit the surface in the same direction again.