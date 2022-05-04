/**********************************************************************************************/
/*                                  SNOWCOOKIE - SCE-CORELIB                                  */
/*--------------------------------------------------------------------------------------------*/
/*    | PROJECT INFO |                                                                        */
/*--------------------------------------------------------------------------------------------*/
/*        | Solution Name:    Snowcookie                                             |        */
/*        | Project Name:     sce-corelib                                            |        */
/*        | File Name:        sce-platform.h                                         |        */
/*                                                                                            */
/*        | Description:      Obtains system hardware and software information       |        */
/*        |                   required for compiling the engine. Validating					  */
/*        |                   architechture,										 |		  */
/*--------------------------------------------------------------------------------------------*/
/*    | SYSTEM INFO |                                                                         */
/*--------------------------------------------------------------------------------------------*/
/*        | HostName:         ctos01                                                 |        */
/*        | UserName:         CTOS01\chloe                                           |        */
/*        | System Time:      07/02/2022 23:41:33                                    |        */
/*--------------------------------------------------------------------------------------------*/
/*    | AUTHOR INFO |                                                                         */
/*--------------------------------------------------------------------------------------------*/
/*        | Author Name:      Chloe Turner                                           |        */
/*        | Date:             07/02/2022                                             |        */
/*        | Copyright:        Copyright 2022 Chloe Turner                            |        */
/**********************************************************************************************/

#ifdef DEBUG

#endif // DEBUG

#if defined(__sce_no_client__)
	#define sce_api __declspec(dllexport)
#else
	#define sce_api __declspec(dllexport)
#endif

//// Operating System Selection
//#pragma region 
//// Windows - Operating System
//#ifdef _WIN64 || _WIN32
//constexpr auto __windows__ = (1 << 0)	// define operating system;
//#endif // WIN32
//#pragma endregion
