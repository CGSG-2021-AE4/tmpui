/***************************************************************
 * Copyright (C) 2021-2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tmp_ui_mp_value.h
 * PURPOSE     : Tough Megapolis Planner project.
 *               file for mapper value in mapper system.
 * PROGRAMMER  : CGSG'2022.
 *               Ilias Safin,
 * LAST UPDATE : 22.12.2022
 * NOTE        : Namespace 'ui', 'mapper'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tmp_ui_mp_value_h_
#define __tmp_ui_mp_value_h_

#include "tmp_ui_mp_manager.h"

namespace tmp
{
  namespace ui
  {
    namespace mapper
    {
      // class mapper value fore using in programm
      template <typename Type>
        class mp_value
        {
          mp_variable *Variable; // mapper value variable
        public:
          /* mp_value class constructor function.
           * ARGUMENTS:
           *    - mapper value name:
           *       const std::string MapName;
           *    - mapper value variable value:
           *       const Type Var;
           *    - has mapper bolder:
           *      BOOL HasBolder;
           *    - mapper value variable min, max, default value:
           *       const Type Mi, Ma, Def;
           */
          mp_value( const std::string &MapName, const Type &Var, BOOL HasBolder, const Type &Mi, const Type &Ma, const Type &Def )
          {
            if (!mapper_manager::Get().FindVariable(MapName))
            {
              Variable = new mp_variable(MapName, Var, HasBolder, Mi, Ma, Def);
              mapper_manager::Get().MP_GlobalArray.push_back(Variable);
            }
            else
              Variable = mapper_manager::Get().GetVariable(MapName);
          } /* End of 'mp_value' class constructor */

          /* mp_value class constructor function(Using for AE4 !!! only).
           * ARGUMENTS:
           *    - mapper value name:
           *       const std::string MapName;
           */
          mp_value( const std::string &MapName )
          {
            if (!mapper_manager::Get().FindVariable(MapName))
            {
              Variable = new mp_variable(MapName, Type(), Type(), Type(), Type());
              mapper_manager::Get().MP_GlobalArray.push_back(Variable);
            }
            else
              Variable = mapper_manager::Get().GetVariable(MapName);
          } /* End of 'mp_value' class constructor */

          /* Operator = function.
           * ARGUMENTS:
           *   - value for assignment:
           *       const Type &Val;
           * RETURNS:
           *   (variable &) self reference.
           */
          inline mp_value & operator=( const Type &Val )
          {
            Variable->OldValue = Variable->Get<Type>();
            Variable->HasChanged = TRUE;

            if (Variable->IsHasBolder)
              if (Variable->Max.Get<Type>() < Val)
                Variable->Variable = Variable->Max;
              else if (Variable->Min.Get<Type>() > Val)
                Variable->Variable = Variable->Min;
              else Variable->Variable = Val;
            else
              Variable->Variable = Val;

            return *this;
          } /* End of 'operator=' function */

          /* Cast to type Type & function.
           * ARGUMENTS: None.
           */
          operator Type & ( void )
          {
            Variable->OldValue = Variable->Get<Type>();

            return Variable->Get<Type>();
          } /* End fo 'Type &' function */

          /* Check has variable value changed function.
           * ARGUMENTS: None.
           * RETURNS:
           *   (BOOL) is variable changed.
           */
          BOOL IsChanged( VOID )
          {
            return Variable->HasChanged, Variable->HasChanged = FALSE;
          } /* End of 'IsChanged' function */
        }; /* End of 'mp_value' class */
    } /* End of 'mapper' namespace */
  } /* End of 'ui' namespace */
} /* End of 'tmp' namespace */
#endif /* __tmp_ui_mp_value_h_ */

/* END OF 'tmp_ui_mp_value.h' FILE */
