/***************************************************************
 * Copyright (C) 2021-2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tmp_ui_mp_manager.h
 * PURPOSE     : Tough Megapolis Planner project.
 *               file for mapper manager in mapper system.
 * PROGRAMMER  : CGSG'2022.
 *               Ilias Safin,
 * LAST UPDATE : 22.12.2022
 * NOTE        : Namespace 'ui', 'mapper'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tmp_ui_mp_manager_h_
#define __tmp_ui_mp_manager_h_

#include "tmp_ui_mp_variable.h"

namespace tmp
{
  namespace ui
  {
    namespace mapper
    {
      // class mapper manager for ui draw system and mapper system interaction
      class mapper_manager
      {
        /* Variable class destructor function.
         * ARGUMENTS: None.
         */
        ~mapper_manager( VOID )
        {
          for (auto Mp : MP_GlobalArray)
            delete Mp;
        } /* End of 'mapper_manager' class destructor */

      public:
        std::vector<mp_variable *> MP_GlobalArray; // mapper variable array for interaction with ui draw system

        /* Static get self reference function.
         * ARGUMENTS: None.
         * RETURNS:
         *   (mapper_manager &) self reference.
         */
        static mapper_manager & Get( VOID )
        {
          static mapper_manager Manager;

          return Manager;
        } /* End of 'Get' function */

        /* Find mapper variable in array function.
         * ARGUMENTS:
         *   - mapper variable name:
         *       std::string Name;
         * RETURNS:
         *   (BOOL) 1 if variable is in array, 0 if variable is not in array.
         */
        BOOL FindVariable( std::string Name )
        {
          for (auto *Var : MP_GlobalArray)
          {
            if (Var->MapperName == Name )
              return TRUE;
          }
          return FALSE;
        } /* End of 'FindVariable' function */

        /* Get mapper variable in array function.
         * ARGUMENTS:
         *   - mapper variable name:
         *       std::string Name;
         * RETURNS:
         *   (mp_variable *) reference to mapper variable in array.
         */
        mp_variable * GetVariable( std::string Name )
        {
          for (auto Var : MP_GlobalArray)
          {
            if (Var->MapperName == Name )
              return Var;
          }
          return nullptr;
        } /* End of 'GetVariable' function */
      }; /* End of 'mapper_maneger' class */
    } /* End of 'mapper' namespace */
  } /* End of 'ui' namespace */
} /* End of 'tmp' namespace */
#endif /* __tmp_ui_mp_manager_h_ */

/* END OF 'tmp_ui_mp_manager.h' FILE */
