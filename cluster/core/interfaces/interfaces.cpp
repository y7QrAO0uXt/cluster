﻿#include "interfaces.h"

bool c_interfaces::capture_all( ) {
	client = this->capture_interface<i_client>( "client.dll", "VClient018" );
	if ( !client )
		return false;

	entity_list = this->capture_interface<i_entity_list>( "client.dll", "VClientEntityList003" );
	if ( !entity_list )
		return false;

	client_mode = **reinterpret_cast< i_client_mode*** >( ( *reinterpret_cast< uintptr_t** >( client ) )[ 10 ] + 5 );
	if ( !client_mode ) {
		g_console.error( "failed to capture client_mode from client.dll" );
		return false;
	}

	g_console.message( "captured client_mode from client.dll" );

	globals = **reinterpret_cast< i_globals*** >( ( *reinterpret_cast< uintptr_t** >( client ) )[ 11 ] + 10 );
	if ( !globals ) {
		g_console.error( "failed to capture globals from client.dll" );
		return false;
	}

	g_console.message( "captured globals from client.dll" );

	std::optional<std::uint8_t*> client_state_signature = g_utilities.signature_scan( "engine.dll", "A1 ? ? ? ? 8B 80 ? ? ? ? C3" );
	if ( !client_state_signature.has_value( ) ) {
		g_console.error( "failed to capture client_state from engine.dll" );
		return false;
	}

	client_state = **reinterpret_cast< i_client_state*** >( client_state_signature.value( ) + 1 );
	if ( !client_state ) {
		g_console.error( "failed to capture client_state from engine.dll" );
		return false;
	}

	g_console.message( "captured client_state from engine.dll" );

	panel = this->capture_interface<i_panel>( "vgui2.dll", "VGUI_Panel009" );
	if ( !panel )
		return false;

	std::optional<std::uint8_t*> direct_x_signature = g_utilities.signature_scan( "shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C" );
	if ( !direct_x_signature.has_value( ) ) {
		g_console.error( "failed to capture direct_x from shaderapidx9.dll" );
		return false;
	}

	direct_x = **reinterpret_cast< IDirect3DDevice9*** >( direct_x_signature.value( ) + 1 );
	if ( !direct_x ) {
		g_console.error( "failed to capture direct_x from shaderapidx9.dll" );
		return false;
	}

	g_console.message( "captured direct_x from shaderapidx9.dll" );

	file_system = this->capture_interface<void*>( "filesystem_stdio.dll", "VFileSystem017" );
	if ( !file_system )
		return false;

	model_render = this->capture_interface<i_model_render>( "engine.dll", "VEngineModel016" );
	if ( !model_render )
		return false;

	return true;
}

c_interfaces g_interfaces;
