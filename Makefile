BUILD_DIR ?= build-make-ninja
APP ?= vision_parte1
CMAKE_ARGS ?=

ifeq ($(OS),Windows_NT)
SHELL := cmd.exe
.SHELLFLAGS := /C
OPENCV_ROOT ?= C:/msys64/ucrt64
OPENCV_DIR ?= $(OPENCV_ROOT)/lib/cmake/opencv5
export PATH := $(OPENCV_ROOT)/bin;$(PATH)
CONFIG = -G Ninja -DCMAKE_CXX_COMPILER="$(OPENCV_ROOT)/bin/g++.exe" -DOpenCV_DIR="$(OPENCV_DIR)"
EXE = .exe
endif

.PHONY: all configure run

all: configure
	cmake --build "$(BUILD_DIR)"

configure:
	cmake -S . -B "$(BUILD_DIR)" $(CONFIG) $(CMAKE_ARGS)

run: all
	"$(BUILD_DIR)/$(APP)$(EXE)"
