# Ray tracer 

Another re-implementation of Peter Shirley's *Ray Tracing in One Weekend* with the following changes:
* use `stb_image` for creating .png images instead of .pem
* use [Eigen](http://eigen.tuxfamily.org) rather than a naive vec3 class
* store normalized `Ray` directions
