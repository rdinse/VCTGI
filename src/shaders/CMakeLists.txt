project(SHADERS)

file(GLOB_RECURSE GLSL *.glsl)
file(GLOB_RECURSE VERT *.vert *.vertex *.vs)
file(GLOB_RECURSE TCS *.tcs *.cont *.t_cont)
file(GLOB_RECURSE TES *.tes *.eval *.t_eval)
file(GLOB_RECURSE GEOM *.geom *.geo *.geometry *.gs)
file(GLOB_RECURSE FRAG *.frag *.fragment *.fs *.ps)
file(GLOB_RECURSE COMP *.comp *.compute *.cs)

SOURCE_GROUP(Vertex-Shaders FILES ${VERT})
SOURCE_GROUP(Tessellation-Control-Shaders FILES ${TCS})
SOURCE_GROUP(Tessellation-Evaluation-Shaders FILES ${TES})
SOURCE_GROUP(Geometry-Shaders FILES ${GEOM})
SOURCE_GROUP(Fragment-Shaders FILES ${FRAG})
SOURCE_GROUP(Compute-Shaders FILES ${COMP})

add_custom_target(SHADERS SOURCES ${GLSL} ${VERT} ${TCS} ${TES} ${GEOM} ${FRAG} ${COMP})