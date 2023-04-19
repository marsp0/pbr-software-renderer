## TODO

- [ ] 19.04.2023
	- [ ] see if it makes sense to unify display and input into "platform"
	- [ ] handle "hold down" keys (for movement etc.)
	- [ ] remove color type
- [ ] 17.04.2023
	- [ ] add support for camera in gltf.
	- [ ] add support for light settings in gltf. This might require extension
- [ ] 13.04.2023
	- [ ] allow for multiple meshes to be rendered
	- [ ] parse multiple textures in parallel
- [ ] 11.04.2023 
	- [ ] separate mesh from textures. There can be a texture manager that handles all textures 
	and meshes can request the textures they need. (or similar)
	- [ ] Machinary blog explained how they do not permit including headers in headers (except for stdint). See if possible to do the same
	- [ ] if multiple nodes share a mesh then it will be duplicated. Should meshes share geometry info like textures? If we dont allow mesh edit then perhaps they should share.
	- [ ] Should static variables be grouped ? buffers + cursor + size etc.
	- [ ] start using gcov to check code coverage and also detect dead/unused code.
- [x] 10.04.2023
	- [x] there is a test in test_json.c that fails ocasionally. (15.04.2023)
