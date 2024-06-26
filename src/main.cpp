/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : main.cpp
 * PURPOSE     : Raytracing project.
 *               Main startup module.
 * PROGRAMMER  : CGSG-SummerCamp'2022.
 *               Andrey Egorov.
 * LAST UPDATE : 23.07.2022.
 * NOTE        : Module namespace 'tmp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "def.h"
#include "win/win.h"

#include "utilities/nn2/nn2.h"

/* The main program function.
 * ARGUMENTS:
 *   - handle of application instance:
 *       HINSTANCE hInstance;
 *   - dummy handle of previous application instance (not used):
 *       HINSTANCE hPrevInstance;
 *   - command line string:
 *       CHAR *CmdLine;
 *   - show window command parameter (see SW_***):
 *       INT CmdShow;
 * RETURNS:
 *   (INT) Error level for operation system (0 for success).
 */
INT WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                    _In_ CHAR *CmdLine, _In_ INT ShowCmd )
{
  AllocConsole();
  freopen("conin$","r",stdin);
  freopen("conout$","w",stdout);
  freopen("conout$","w",stderr);

  nn2::net_config C {nn2::net_arc({1, 1})};
  nn2::net_machine M {};

  const FLT Scale = 10;
  C.Fill({-Scale, Scale}, {-Scale, Scale});
  M.Setup(C);
  M.SetInput({0});

  for (FLT i = 0; i < 10; i += 0.2)
  {
    M.Reset();
    M.SetInput({i});
    M.Dispatch();
    FLT r = M.GetOutput()[0];

    std::cout << "IN: " << i << "  OUT: " << r << "\n";
  }






  tmp::rtwin myw(hInstance);

  myw.Run();
} /* End of 'WinMain' function */
