package com.AIRDiscord
{
	import flash.events.EventDispatcher;
	import flash.events.IEventDispatcher;
	import flash.events.StatusEvent;
	import flash.external.ExtensionContext;
	import flash.events.Event;

	public class AIRDiscordWrapper extends EventDispatcher
	{
		// Our extension context
		private var m_extContext : ExtensionContext = null;

		// The extension ID needs to be a unique string:
		private static const EXTENSION_ID : String = "com.AIRDiscord.extensions";

		public function AIRDiscordWrapper( _target : IEventDispatcher = null )
		{
			super( _target );

			// Create a new extension context and listen for status events from it:
			m_extContext = ExtensionContext.createExtensionContext( EXTENSION_ID, null );
			m_extContext.addEventListener( StatusEvent.STATUS, onStatusEvent );				
		}

		private function onStatusEvent( _event : StatusEvent ) : void
		{
			// React to events sent by the C++ dll
			switch(_event.code)
			{
				case "USER_UPDATED":
					// Send the event again but with different case
					this.dispatchEvent(new Event("User_Updated"));
					break;
				default:
					break;
			}

			// Log these stuff for educational purposes
			trace( "Event code: " + _event.code + ", event level: " + _event.level );
		}

		public function InitDiscord(appId : String) : int
		{
			return m_extContext.call("as_InitDiscord", appId) as int;
		}

		public function GetDiscordUser() : String
		{
			return m_extContext.call("as_GetCurrentUser") as String;
		}

		public function RunDiscordCallbacks() : int
		{
			return m_extContext.call("as_RunDiscordCallbacks") as int;
		}
	}
}
