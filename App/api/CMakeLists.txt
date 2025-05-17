set(APP_NAME cm_api)

add_library(${APP_NAME} STATIC
    src/http_server.cpp
    src/dbus_consumer.cpp
    src/mosquitto_mqtt_subscriber.cpp
    src/posix_message_queue_consumer.cpp
)

target_link_libraries(cm_api PUBLIC
 glog::glog 
 ${MOSQUITTOPP_LIBRARIES} 
 ${MOSQUITTO_LIBRARIES} 
 ${SDBUSCPP_INCLUDE_DIRS}
 )