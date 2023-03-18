cmake_minimum_required(VERSION 3.2)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(vita-hello-triangle)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(SDL2_DIR "${VITASDK}/arm-vita-eabi/lib/cmake/SDL2/")

find_package(SDL2 REQUIRED)


set(VITA_APP_NAME "Hello Triangle")
set(VITA_TITLEID  "REDI00001")
set(VITA_VERSION  "01.00")


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME}
  src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
  SDL2::SDL2
  vitaGL
  SceShaccCgExt
  mathneon
  vitashark
  SceDisplay_stub
	SceGxm_stub
	ScePgf_stub
	ScePvf_stub
	SceSysmodule_stub
	SceCommonDialog_stub
	SceKernelThreadMgr_stub
	SceCtrl_stub
	SceKernelDmacMgr_stub
	SceTouch_stub
	SceAudio_stub
	ScePower_stub
	SceNet_stub
	SceAppMgr_stub
	SceAppUtil_stub
	SceShaccCg_stub
	SceVshBridge_stub
  SceShaccCgExt
	taihen_stub
)

vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME})
vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
  FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
  FILE vertex.vert vertex.vert
  FILE fragment.frag fragment.frag
)
