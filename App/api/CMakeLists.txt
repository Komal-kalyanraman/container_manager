# CMake configuration for cm_api static library (Container Manager)
# Provides protocol consumers for REST/HTTP, POSIX Message Queue, D-Bus, and MQTT.

set(APP_NAME cm_api)

# Core source files (always included)
set(SRC_LIST src/http_server.cpp src/posix_message_queue_consumer.cpp)

# Add D-Bus consumer if enabled
if(ENABLE_DBUS)
    list(APPEND SRC_LIST src/dbus_consumer.cpp)
endif()

# Add MQTT consumer if enabled
if(ENABLE_MQTT)
    list(APPEND SRC_LIST src/mosquitto_mqtt_subscriber.cpp)
endif()

add_library(${APP_NAME} STATIC ${SRC_LIST})

# Always link glog for logging
target_link_libraries(${APP_NAME} PUBLIC glog::glog)

# Link protocol-specific libraries as needed
if(ENABLE_MQTT)
    target_link_libraries(${APP_NAME} PUBLIC ${MOSQUITTOPP_LIBRARIES} ${MOSQUITTO_LIBRARIES})
endif()

if(ENABLE_DBUS)
    target_link_libraries(${APP_NAME} PUBLIC ${SDBUSCPP_LIBRARIES})
endif()

if(ENABLE_ENCRYPTION)
    find_package(OpenSSL REQUIRED)
    target_link_libraries(${APP_NAME} PUBLIC OpenSSL::Crypto)
endif()

# Link security module for encryption support
target_link_libraries(${APP_NAME} PUBLIC cm_security)