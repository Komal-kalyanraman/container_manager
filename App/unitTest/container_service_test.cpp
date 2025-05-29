#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <string>
#include <nlohmann/json.hpp>
#include "inc/command_factory.hpp"
#include "inc/container_service.hpp"
#include "inc/database_interface.hpp"
#include "inc/container_request.hpp"

// Mock implementation of IDatabaseHandler matching the interface
class MockDatabaseHandler : public IDatabaseHandler {
public:
    MOCK_METHOD(void, SaveJson, (const std::string&, const nlohmann::json&), (override));
    MOCK_METHOD(nlohmann::json, GetJson, (const std::string&), (override));
    MOCK_METHOD(void, ClearDatabase, (), (override));
    MOCK_METHOD(void, UpdateField, (const std::string&, const std::string&, const std::string&), (override));
    MOCK_METHOD(void, RemoveKey, (const std::string&), (override));
};

class ContainerServiceHandlerTest : public ::testing::Test {
protected:
    void SetUp() override {
        db_handler = std::make_shared<MockDatabaseHandler>();
        service = std::make_unique<ContainerServiceHandler>(*db_handler);
    }

    std::shared_ptr<MockDatabaseHandler> db_handler;
    std::unique_ptr<ContainerServiceHandler> service;
};

using ::testing::_;
using ::testing::Return;
using ::testing::StrEq;

TEST_F(ContainerServiceHandlerTest, HandleRequest_CreateContainer_Success) {
    ContainerRequest req;
    req.operation = CommandName::CreateContainer;
    req.container_name = "test_container";

    // Expect status update to "created"
    EXPECT_CALL(*db_handler, UpdateField(StrEq("test_container"), StrEq("status"), StrEq("created")));

    // Mock ContainerOperations to return success
    class TestService : public ContainerServiceHandler {
    public:
        using ContainerServiceHandler::ContainerServiceHandler;
        Status ContainerOperations(const ContainerRequest&) { return Status::Ok(); }
    };
    TestService test_service(*db_handler);

    auto result = test_service.HandleRequest(req);
    EXPECT_EQ(result["status"], "success");
    EXPECT_EQ(result["container"], "test_container");
}

TEST_F(ContainerServiceHandlerTest, HandleRequest_CreateContainer_Failure) {
    ContainerRequest req;
    req.operation = CommandName::CreateContainer;
    req.container_name = "test_container";

    // Expect status update to "Error creating container"
    EXPECT_CALL(*db_handler, UpdateField(StrEq("test_container"), StrEq("status"), StrEq("Error creating container")));

    // Mock ContainerOperations to return error
    class TestService : public ContainerServiceHandler {
    public:
        using ContainerServiceHandler::ContainerServiceHandler;
        Status ContainerOperations(const ContainerRequest&) { return Status::Error(StatusCode::InternalError, "fail"); }
    };
    TestService test_service(*db_handler);

    auto result = test_service.HandleRequest(req);
    EXPECT_EQ(result["status"], "error");
    EXPECT_EQ(result["container"], "test_container");
}

TEST_F(ContainerServiceHandlerTest, HandleRequest_RemoveContainer_Success) {
    ContainerRequest req;
    req.operation = CommandName::RemoveContainer;
    req.container_name = "test_container";

    // Expect RemoveKey to be called
    EXPECT_CALL(*db_handler, RemoveKey(StrEq("test_container")));

    class TestService : public ContainerServiceHandler {
    public:
        using ContainerServiceHandler::ContainerServiceHandler;
        Status ContainerOperations(const ContainerRequest&) { return Status::Ok(); }
    };
    TestService test_service(*db_handler);

    auto result = test_service.HandleRequest(req);
    EXPECT_EQ(result["status"], "success");
    EXPECT_EQ(result["container"], "test_container");
}