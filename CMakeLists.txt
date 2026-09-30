cmake_minimum_required(VERSION 3.15)
project(PotentialFlowSolver LANGUAGES CXX)

# Стандарт современного C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск библиотеки Eigen3 в системе
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

# Создание исполняемого файла из main.cpp
add_executable(potential_flow_solver main.cpp)

# Подключение заголовочных файлов из текущей папки (для stb_image.h)
target_include_directories(potential_flow_solver PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Линковка библиотеки Eigen
target_link_libraries(potential_flow_solver PRIVATE Eigen3::Eigen)

# Флаги предупреждений компилятора
if (MSVC)
    target_compile_options(potential_flow_solver PRIVATE /W4)
else()
    target_compile_options(potential_flow_solver PRIVATE -Wall -Wextra -O3)
endif()