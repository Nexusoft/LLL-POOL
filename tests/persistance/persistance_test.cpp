#include <gtest/gtest.h>
#include <sqlite/sqlite3.h>
#include "persistance_fixture.hpp"
#include "persistance/command/command.hpp"
#include <string>

using namespace persistance;
using ::nexuspool::persistance::command::Type;

TEST(Persistance, initialisation)
{
	auto logger = spdlog::stdout_color_mt("test_logger");
	config::Persistance_config config{};
	auto component = persistance::create_component(logger, config);
	EXPECT_TRUE(component);
}

TEST_F(Persistance_fixture, create_shared_data_writer)
{
	auto data_writer_factory = m_persistance_component->get_data_writer_factory();
	EXPECT_TRUE(data_writer_factory);
	auto shared_data_writer = data_writer_factory->create_shared_data_writer();
	EXPECT_EQ(shared_data_writer.use_count(), 2);	// factory stores one shared_ptr and gives one shared_ptr out

	auto shared_data_writer_2 = data_writer_factory->create_shared_data_writer();
	EXPECT_EQ(shared_data_writer_2.use_count(), 3);
}

/*
TEST_F(Persistance_fixture, command_factory_create_all_commands)
{
	std::vector<command::Type> command_types_input = {Type::create_db_schema, Type::get_banned_api_ip, Type::get_banned_user_and_ip};
	auto command_factory = m_persistance_component->get_command_factory();

	for (auto& command_type : command_types_input)
	{
		auto command = command_factory->create_command(command_type);
		EXPECT_EQ(command->get_type(), command_type);
	}
}
*/

TEST_F(Persistance_fixture, command_is_user_and_connection_banned_not_found)
{
	auto data_reader = m_persistance_component->get_data_reader_factory()->create_data_reader();
	for (auto& invalid_input : m_test_data.m_invalid_input)
	{
		auto result = data_reader->is_user_and_connection_banned(invalid_input, invalid_input);
		EXPECT_FALSE(result);
	}

	for (auto& valid_input : m_test_data.m_banned_users_connections_input)
	{
		auto result = data_reader->is_user_and_connection_banned(valid_input.first, valid_input.second);
		EXPECT_TRUE(result);
	}
}

TEST_F(Persistance_fixture, command_is_connection_banned_not_found)
{
	auto data_reader = m_persistance_component->get_data_reader_factory()->create_data_reader();
	for (auto& invalid_input : m_test_data.m_invalid_input)
	{
		auto result = data_reader->is_connection_banned(invalid_input);
		EXPECT_FALSE(result);
	}

	for (auto& valid_input : m_test_data.m_banned_connections_api_input)
	{
		auto result = data_reader->is_connection_banned(valid_input);
		EXPECT_TRUE(result);
	}
}

TEST_F(Persistance_fixture, command_get_latest_blocks)
{
	auto data_reader = m_persistance_component->get_data_reader_factory()->create_data_reader();
	auto result = data_reader->get_latest_blocks();
	EXPECT_FALSE(result.empty());

}