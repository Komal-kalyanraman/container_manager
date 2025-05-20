# ==============================================================================
# App/api/CMakeLists.txt for Container Manager
#
# This CMake configuration builds the cm_api static library, which provides
# protocol consumers for the Container Manager backend (HTTP server, POSIX Message Queue,
# D-Bus, and MQTT). Source files and dependencies are included conditionally
# based on the enabled protocol flags, so you only need to install and build
# what you require.
#
# Usage:
#   - To enable/disable protocols, set the ENABLE_* flags in the top-level CMake.
#   - Only the required source files and libraries for enabled protocols are included.
#   - Example: To build only with D-Bus, use -DENABLE_DBUS=ON -DENABLE_MQTT=OFF, etc.
# ==============================================================================

set(APP_NAME cm_api)

set(SRC_LIST src/http_server.cpp src/posix_message_queue_consumer.cpp)

if(ENABLE_DBUS)
    list(APPEND SRC_LIST src/dbus_consumer.cpp)
endif()

if(ENABLE_MQTT)
    list(APPEND SRC_LIST src/mosquitto_mqtt_subscriber.cpp)
endif()

add_library(${APP_NAME} STATIC ${SRC_LIST})

target_link_libraries(${APP_NAME} PUBLIC glog::glog)

if(ENABLE_MQTT)
    target_link_libraries(${APP_NAME} PUBLIC ${MOSQUITTOPP_LIBRARIES} ${MOSQUITTO_LIBRARIES})
endif()

if(ENABLE_DBUS)
    target_link_libraries(${APP_NAME} PUBLIC ${SDBUSCPP_LIBRARIES})
endif()