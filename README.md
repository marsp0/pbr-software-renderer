# PBR Software Renderer

TODO: nice description

## Features

## References

The references are pretty much the same as the [Phong Software Renderer](https://github.com/marsp0/phong-software-renderer) and can be found [here](https://github.com/marsp0/software-renderer/wiki/References)

## Code flow

the above diagram shows how the execution of the program flows

``` mermaid
%%{init: { 'theme':'forest' } }%%
sequenceDiagram
    autonumber
    main ->>+ renderer: renderer_new
    
    renderer ->>+ frame_buffer: frame_buffer_new
    frame_buffer -->>- renderer: return

    renderer ->>+ depth_buffer: depth_buffer_new
    depth_buffer -->>- renderer: return

    renderer ->>+ file_parser: parse_scene 
    file_parser -->>- renderer: return

    renderer -->>- main: return

    main ->>+ renderer: renderer_run

    loop render loop

        renderer ->>+ display: handle_input
        display -->>- renderer: return

        loop for each model

            renderer ->>+ vertex_processor: vertex_processor_process
            note over vertex_processor, renderer: split triangles in buckets and process in parallel
            vertex_processor -->>- renderer: wait for threads to finish

            renderer ->>+ vertex_processor: vertex_processor_combine
            note over vertex_processor, renderer: note: measure if parallel process + combine is faster than sequential processing
            vertex_processor -->>- renderer: return

            renderer ->>+ fragment_processor: fragment_processor_process
            note over renderer, fragment_processor: process in parallel
            note over renderer, fragment_processor: each thread iterates over the render buffer
            note over renderer, fragment_processor: and renders only the triangles that are within its AABB
            fragment_processor -->>- renderer: wait for threads to finish

        end

        renderer ->>+ display: display_draw
        display -->>- renderer: return

    end

    renderer -->>- main: return

    main ->>+ renderer: renderer_free
    renderer -->>- main: return

```