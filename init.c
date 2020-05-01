void SpawnObject(string objectName, vector position, vector orientation)
{
    Object obj;
    obj = Object.Cast(GetGame().CreateObject(objectName, "0 0 0"));
    obj.SetPosition(position);
    obj.SetOrientation(orientation);
    obj.SetOrientation( obj.GetOrientation() ); //Collision fix
    obj.Update();
    obj.SetAffectPathgraph( true, false );
    if( obj.CanAffectPathgraph() ) GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().UpdatePathgraphRegionByObject, 100, false, obj );
}

//#include "$CurrentDir:mpmissions\\dayzoffline.chiemsee\\BBPCementMixers.c"

void main()
{
	//INIT WEATHER BEFORE ECONOMY INIT------------------------
	Weather weather = g_Game.GetWeather();

	weather.MissionWeather(false);    // false = use weather controller from Weather.c

	weather.GetOvercast().Set( Math.RandomFloatInclusive(0.4, 0.6), 1, 0);
	weather.GetRain().Set( 0, 0, 1);
	weather.GetFog().Set( Math.RandomFloatInclusive(0.05, 0.1), 1, 0);

	//INIT ECONOMY--------------------------------------
	Hive ce = CreateHive();
	if ( ce )
		ce.InitOffline();


	//DATE RESET AFTER ECONOMY INIT-------------------------
	int year, month, day, hour, minute;
	int reset_month = 9, reset_day = 20;
	GetGame().GetWorld().GetDate(year, month, day, hour, minute);

	if ((month == reset_month) && (day < reset_day))
	{
		GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
	}
	else
	{
		if ((month == reset_month + 1) && (day > reset_day))
		{
			GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
		}
		else
		{
			if ((month < reset_month) || (month > reset_month + 1))
			{
				GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
			}
		}
	}

//BBPCementMixers();

GetCEApi().ExportProxyData( "7500 0 7500", 15000 );			// Generate mapgrouppos.xml
GetCEApi().ExportClusterData();								// Generate mapgroupcluster.xml
}

class CustomMission: MissionServer
{
	void SetRandomHealth(EntityAI itemEnt)
	{
		if ( itemEnt )
		{
			int rndHlt = Math.RandomInt(55,100);
			itemEnt.SetHealth("","",rndHlt);
		}
	}

	override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
	{
		Entity playerEnt;
		playerEnt = GetGame().CreatePlayer(identity, characterName, pos, 0, "NONE");//Creates random player
		Class.CastTo(m_player, playerEnt);
		
		GetGame().SelectPlayer(identity, m_player);
		
		return m_player;
	}
	
	override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
	{

		player.RemoveAllItems();

		EntityAI itemEnt;
		ItemBase itemBs;
		
		// Setup custom classes
		switch (Math.RandomInt(0, 8)) { 
		case 0:
		// Soldier
		player.GetInventory().CreateInInventory("CombatKnife");
		break; 
		case 1: 
		// Paramedic
		player.GetInventory().CreateInInventory("SalineBagIV");
		break; 
		case 2: 
		// Office worker
		player.GetInventory().CreateInInventory("SodaCan_Cola");
		break; 
		case 3: 
		// Biker
		player.GetInventory().CreateInInventory("Matchbox");
		break; 
		case 4: 
		// Hiker
		player.GetInventory().CreateInInventory("Compass");
		break; 
		case 5: 
		// Cop
		player.GetInventory().CreateInInventory("Flashlight");
		player.GetInventory().CreateInInventory("Battery9V");
		break; 
		case 6: 
		// Lumberjack
		player.GetInventory().CreateInInventory("Apple");
		break; 
		case 7: 
		// Hood
		player.GetInventory().CreateInInventory("KitchenKnife");
		break; 
		case 8: 
		// Fireman
		player.GetInventory().CreateInInventory("FirefighterAxe");
		break; 
	}
	
	//Give universal gear
	//player.GetInventory().CreateInInventory("Military_Sweater_Black");
	//player.GetInventory().CreateInInventory("Kneepads_Jeans_Black");
	//player.GetInventory().CreateInInventory("High_Knee_Sneakers");
	player.GetInventory().CreateInInventory("Chemlight_White");

	itemEnt = player.GetInventory().CreateInInventory("Rag");
	itemBs = ItemBase.Cast(itemEnt);
	itemBs.SetQuantity(10);

	}
};

Mission CreateCustomMission(string path)
{
	return new CustomMission();
}