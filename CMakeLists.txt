cmake_minimum_required(VERSION 3.25)

project(RaytracerFromScratch
		VERSION 
			1.0.0
		DESCRIPTION 
			"Raytracer built from scratch"
		HOMEPAGE_URL 
			"https://github.com/wikko75/Raytraycer-From-Scratch"
		LANGUAGES 
			CXX
) 

add_executable(${PROJECT_NAME})

set_target_properties(
	${PROJECT_NAME} 
		PROPERTIES 
			CXX_STANDARD 23
			CXX_STANDARD_REQUIRED ON
)

add_subdirectory(src)

target_include_directories(${PROJECT_NAME} PRIVATE inc)