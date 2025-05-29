#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <string>
#include <nlohmann/json.hpp>
#include "inc/command_factory.hpp"
#include "inc/container_service.hpp"
#include "inc/database_interface.hpp"
#include "inc/container_request.hpp"
#include "inc/common.hpp"

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
    }
    std::shared_ptr<MockDatabaseHandler> db_handler;
};

using ::testing::_;
using ::testing::Return;
using ::testing::StrEq;

TEST_F(ContainerServiceHandlerTest, HandleRequest_CreateContainer_Success) {
    class TestService : public ContainerServiceHandler {
    public:
        using ContainerServiceHandler::ContainerServiceHandler;
        Status ContainerOperations(const ContainerRequest&) override { return Status::Ok(); }
    };

    ContainerRequest req;
    req.operation = CommandName::CreateContainer;
    req.container_name = "test_container";

    // Expect status update to "created"
    EXPECT_CALL(*db_handler, UpdateField(StrEq("test_container"), StrEq("status"), StrEq("created")));

    TestService test_service(*db_handler);
    auto result = test_service.HandleRequest(req);
    EXPECT_EQ(result["status"], "success");
    EXPECT_EQ(result["container"], "test_container");
}

TEST_F(ContainerServiceHandlerTest, HandleRequest_CreateContainer_Failure) {
    class FailingService : public ContainerServiceHandler {
    public:
        using ContainerServiceHandler::ContainerServiceHandler;
        Status ContainerOperations(const ContainerRequest&) override { return Status::Error(StatusCode::NotFound, "fail"); }
    };

    ContainerRequest req;
    req.operation = CommandName::CreateContainer;
    req.container_name = "test_container";

    // Expect status update to "Error creating container"
    EXPECT_CALL(*db_handler, UpdateField(StrEq("test_container"), StrEq("status"), StrEq("Error creating container")));

    FailingService test_service(*db_handler);
    auto result = test_service.HandleRequest(req);
    EXPECT_EQ(result["status"], "error");
    EXPECT_EQ(result["container"], "test_container");
}

TEST_F(ContainerServiceHandlerTest, HandleRequest_StartContainer_Success) {
    class TestService : public ContainerServiceHandler {
    public:
        using ContainerServiceHandler::ContainerServiceHandler;
        Status ContainerOperations(const ContainerRequest&) override { return Status::Ok(); }
    };

    ContainerRequest req;
    req.operation = CommandName::StartContainer;
    req.container_name = "test_container";

    // Expect status update to "created"
    EXPECT_CALL(*db_handler, UpdateField(StrEq("test_container"), StrEq("status"), StrEq("running")));

    TestService test_service(*db_handler);
    auto result = test_service.HandleRequest(req);
    EXPECT_EQ(result["status"], "success");
    EXPECT_EQ(result["container"], "test_container");
}

TEST_F(ContainerServiceHandlerTest, HandleRequest_StartContainer_Failure) {
    class FailingService : public ContainerServiceHandler {
    public:
        using ContainerServiceHandler::ContainerServiceHandler;
        Status ContainerOperations(const ContainerRequest&) override { return Status::Error(StatusCode::InvalidArgument, "fail"); }
    };

    ContainerRequest req;
    req.operation = CommandName::StartContainer;
    req.container_name = "test_container";

    // Expect status update to "Error creating container"
    EXPECT_CALL(*db_handler, UpdateField(StrEq("test_container"), StrEq("status"), StrEq("Error running container")));

    FailingService test_service(*db_handler);
    auto result = test_service.HandleRequest(req);
    EXPECT_EQ(result["status"], "error");
    EXPECT_EQ(result["container"], "test_container");
}

TEST_F(ContainerServiceHandlerTest, HandleRequest_StopContainer_Success) {
    class TestService : public ContainerServiceHandler {
    public:
        using ContainerServiceHandler::ContainerServiceHandler;
        Status ContainerOperations(const ContainerRequest&) override { return Status::Ok(); }
    };

    ContainerRequest req;
    req.operation = CommandName::StopContainer;
    req.container_name = "test_container";

    // Expect status update to "created"
    EXPECT_CALL(*db_handler, UpdateField(StrEq("test_container"), StrEq("status"), StrEq("stopped")));

    TestService test_service(*db_handler);
    auto result = test_service.HandleRequest(req);
    EXPECT_EQ(result["status"], "success");
    EXPECT_EQ(result["container"], "test_container");
}

TEST_F(ContainerServiceHandlerTest, HandleRequest_StopContainer_Failure) {
    class FailingService : public ContainerServiceHandler {
    public:
        using ContainerServiceHandler::ContainerServiceHandler;
        Status ContainerOperations(const ContainerRequest&) override { return Status::Error(StatusCode::InternalError, "fail"); }
    };

    ContainerRequest req;
    req.operation = CommandName::StopContainer;
    req.container_name = "test_container";

    // Expect status update to "Error creating container"
    EXPECT_CALL(*db_handler, UpdateField(StrEq("test_container"), StrEq("status"), StrEq("Error stopping container")));

    FailingService test_service(*db_handler);
    auto result = test_service.HandleRequest(req);
    EXPECT_EQ(result["status"], "error");
    EXPECT_EQ(result["container"], "test_container");
}

TEST_F(ContainerServiceHandlerTest, HandleRequest_RestartContainer_Success) {
    class TestService : public ContainerServiceHandler {
    public:
        using ContainerServiceHandler::ContainerServiceHandler;
        Status ContainerOperations(const ContainerRequest&) override { return Status::Ok(); }
    };

    ContainerRequest req;
    req.operation = CommandName::RestartContainer;
    req.container_name = "test_container";

    // Expect status update to "created"
    EXPECT_CALL(*db_handler, UpdateField(StrEq("test_container"), StrEq("status"), StrEq("running")));

    TestService test_service(*db_handler);
    auto result = test_service.HandleRequest(req);
    EXPECT_EQ(result["status"], "success");
    EXPECT_EQ(result["container"], "test_container");
}

TEST_F(ContainerServiceHandlerTest, HandleRequest_RestartContainer_Failure) {
    class FailingService : public ContainerServiceHandler {
    public:
        using ContainerServiceHandler::ContainerServiceHandler;
        Status ContainerOperations(const ContainerRequest&) override { return Status::Error(StatusCode::Timeout, "fail"); }
    };

    ContainerRequest req;
    req.operation = CommandName::RestartContainer;
    req.container_name = "test_container";

    // Expect status update to "Error creating container"
    EXPECT_CALL(*db_handler, UpdateField(StrEq("test_container"), StrEq("status"), StrEq("Error restarting container")));

    FailingService test_service(*db_handler);
    auto result = test_service.HandleRequest(req);
    EXPECT_EQ(result["status"], "error");
    EXPECT_EQ(result["container"], "test_container");
}

TEST_F(ContainerServiceHandlerTest, HandleRequest_RemoveContainer_Success) {
    class RemoveService : public ContainerServiceHandler {
    public:
        using ContainerServiceHandler::ContainerServiceHandler;
        Status ContainerOperations(const ContainerRequest&) override { return Status::Ok(); }
    };

    ContainerRequest req;
    req.operation = CommandName::RemoveContainer;
    req.container_name = "test_container";

    // Expect RemoveKey to be called
    EXPECT_CALL(*db_handler, RemoveKey(StrEq("test_container")));

    RemoveService test_service(*db_handler);
    auto result = test_service.HandleRequest(req);
    EXPECT_EQ(result["status"], "success");
    EXPECT_EQ(result["container"], "test_container");
}

TEST_F(ContainerServiceHandlerTest, HandleRequest_RemoveContainer_Failure) {
    class FailingRemoveService : public ContainerServiceHandler {
    public:
        using ContainerServiceHandler::ContainerServiceHandler;
        Status ContainerOperations(const ContainerRequest&) override { return Status::Error(StatusCode::PermissionDenied, "fail"); }
    };

    ContainerRequest req;
    req.operation = CommandName::RemoveContainer;
    req.container_name = "test_container";

    // Expect RemoveKey to be called
    EXPECT_CALL(*db_handler, UpdateField(StrEq("test_container"), StrEq("status"), StrEq("Error removing container")));

    FailingRemoveService test_service(*db_handler);
    auto result = test_service.HandleRequest(req);
    EXPECT_EQ(result["status"], "error");
    EXPECT_EQ(result["container"], "test_container");
}