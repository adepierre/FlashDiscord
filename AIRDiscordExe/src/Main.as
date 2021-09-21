package
{
	import flash.display.Sprite;
	import flash.text.TextField;
    import com.AIRDiscord.AIRDiscordWrapper;
    import flash.utils.Timer;
    import flash.events.TimerEvent;
    import flash.events.Event;

	public class Main extends Sprite
	{

        private var m_ext : AIRDiscordWrapper = null;
        private var timer:Timer;

		public function Main()
		{
			var appId : String = "INSERT_YOUR_DISCORD_APP_ID_HERE";
            m_ext = new AIRDiscordWrapper();
			m_ext.addEventListener("User_Updated", OnDiscordUserUpdated);
			m_ext.InitDiscord(appId);

			timer = new Timer(16);
			timer.addEventListener(TimerEvent.TIMER, OnTimerCallback);
			timer.start();
		}

		private function OnDiscordUserUpdated(event: Event) : void
		{
			var userName : String = m_ext.GetDiscordUser();
			var tf:TextField = new TextField();
			tf.text = "userName: " + userName;
			addChild(tf);
		}

		private function OnTimerCallback(event:TimerEvent) : void
		{
			m_ext.RunDiscordCallbacks();
		}
	}
}