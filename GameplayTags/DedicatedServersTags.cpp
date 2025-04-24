#include "DedicatedServersTags.h"


namespace DedicatedServersTags
{
	namespace GameSessionsAPI
	{
		UE_DEFINE_GAMEPLAY_TAG(ListFleets, "DedicatedServersTags.GameSessionsAPI.ListFleets")
		UE_DEFINE_GAMEPLAY_TAG(FindOrCreateGameSession, "DedicatedServersTags.GameSessionsAPI.FindOrCreateGameSession")
		UE_DEFINE_GAMEPLAY_TAG(CreatePlayerSession, "DedicatedServersTags.GameSessionsAPI.CreatePlayerSession")

	}

	namespace PortalAPI
	{
		UE_DEFINE_GAMEPLAY_TAG(SignUp, "DedicatedServersTags.PortalAPI.SignUp")
		UE_DEFINE_GAMEPLAY_TAG(ConfirmSignUp, "DedicatedServersTags.PortalAPI.ConfirmSignUp")
		UE_DEFINE_GAMEPLAY_TAG(SignIn, "DedicatedServersTags.PortalAPI.SignIn")
	}
	
}

