BUILD_DIR ?= build-make-ninja
APP ?= vision_parte1
CMAKE_ARGS ?=
CMAKE ?= cmake

ifeq ($(OS),Windows_NT)
# Usar el mismo interprete desde GNU Make nativo y desde MSYS2.
SHELL := C:/Windows/System32/WindowsPowerShell/v1.0/powershell.exe
.SHELLFLAGS := -NoProfile -NonInteractive -Command
OPENCV_ROOT ?= C:/msys64/ucrt64
OPENCV_DIR ?= $(OPENCV_ROOT)/lib/cmake/opencv5
# Fijar PATH dentro de PowerShell: MSYS usa otro separador al exportarlo.
WIN_ENV = $$env:Path = '$(OPENCV_ROOT)/bin;' + $$env:Path;
export MSYS2_ARG_CONV_EXCL := *
CONFIG = -G Ninja -DCMAKE_CXX_COMPILER="$(OPENCV_ROOT)/bin/g++.exe" -DOpenCV_DIR="$(OPENCV_DIR)"
EXE = .exe
endif

.PHONY: all configure run

all: configure
	$(WIN_ENV) $(if $(filter Windows_NT,$(OS)),& )"$(CMAKE)" --build "$(BUILD_DIR)" $(if $(filter Windows_NT,$(OS)),; exit $$LASTEXITCODE)

configure:
	$(WIN_ENV) $(if $(filter Windows_NT,$(OS)),& )"$(CMAKE)" -S . -B "$(BUILD_DIR)" $(CONFIG) -DCMAKE_EXPORT_COMPILE_COMMANDS=ON $(CMAKE_ARGS) $(if $(filter Windows_NT,$(OS)),; exit $$LASTEXITCODE)

run: all
	$(WIN_ENV) $(if $(filter Windows_NT,$(OS)),& )"./$(BUILD_DIR)/$(APP)$(EXE)" $(if $(filter Windows_NT,$(OS)),; Write-Host ('Codigo de salida: ' + $$LASTEXITCODE); exit $$LASTEXITCODE)
