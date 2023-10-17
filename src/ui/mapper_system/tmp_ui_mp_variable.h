/***************************************************************
 * Copyright (C) 2021-2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tmp_ui_mp_variable.h
 * PURPOSE     : Tough Megapolis Planner project.
 *               file for mapper variable in mapper system.
 * PROGRAMMER  : CGSG'2022.
 *               Ilias Safin,
 * LAST UPDATE : 20.12.2022
 * NOTE        : Namespace 'ui', 'mapper'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tmp_ui_mp_variable_h_
#define __tmp_ui_mp_variable_h_

#include "tmp_ui_mp_def.h"

namespace tmp
{
  namespace ui
  {
    namespace mapper
    {
      // union for variable value in mapper system
      union variable
      {
        int Int;
        float Float;
        bool Bool;
        vec2 Vec2;
        vec3 Vec3;
        vec4 Vec4;

        /* Variable class constructor function.
         * ARGUMENTS:
         *   - value for one field in union variable:
         *       const Type Val;
         */
        template<typename Type>
          variable( Type Val )
          {
            if constexpr (std::is_same_v<Type, int>)
              Int = Val;
            else if constexpr (std::is_same_v<Type, float>)
              Float = Val;
            else if constexpr (std::is_same_v<Type, bool>)
              Bool = Val;
            else if constexpr (std::is_same_v<Type, vec2>)
              Vec2 = Val;
            else if constexpr (std::is_same_v<Type, vec3>)
              Vec3 = Val;
            else if constexpr (std::is_same_v<Type, vec4>)
              Vec4 = Val;
          } /* End of union 'variable' constructor */

        /* Operator = function.
         * ARGUMENTS:
         *   - value for assignment:
         *       const Type Val;
         * RETURNS:
         *   (variable &) self reference.
         */
        template<typename Type>
          inline variable & operator=( const Type Val )
          {
            if constexpr(std::is_same_v<Type, int>)
              Int = Val;
            else if constexpr(std::is_same_v<Type, float>)
              Float = Val;
            else if constexpr(std::is_same_v<Type, bool>)
              Bool = Val;
            else if constexpr(std::is_same_v<Type, vec2>)
              Vec2 = Val;
            else if constexpr(std::is_same_v<Type, vec3>)
              Vec3 = Val;
            else if constexpr(std::is_same_v<Type, vec4>)
              Vec4 = Val;

            return *this;
          } /* End of 'operator=' function  */

        /* Cast to type Type & function.
         * ARGUMENTS: None.
         */
        template <typename Type>
          operator Type &( VOID )
          {
            if constexpr(std::is_same_v<Type, int>)
              return Int;
            else if constexpr(std::is_same_v<Type, float>)
              return Float;
            else if constexpr(std::is_same_v<Type, bool>)
              return Bool;
            else if constexpr(std::is_same_v<Type, vec2>)
              return Vec2;
            else if constexpr(std::is_same_v<Type, vec3>)
              return Vec3;
            else if constexpr(std::is_same_v<Type, vec4>)
              return Vec4;
            else
              static_assert(FALSE);
          } /* End of 'Type &' function */

        /* Cast to type Type function.
         * ARGUMENTS: None.
         */
        template<typename Type>
          operator Type( VOID )
          {
            if constexpr(std::is_same_v<Type, int>)
              return Int;
            else if constexpr(std::is_same_v<Type, float>)
              return Float;
            else if constexpr(std::is_same_v<Type, bool>)
              return Bool;
            else if constexpr(std::is_same_v<Type, vec2>)
              return Vec2;
            else if constexpr(std::is_same_v<Type, vec3>)
              return Vec3;
            else if constexpr(std::is_same_v<Type, vec4>)
              return Vec4;
            else
              static_assert(FALSE);
          } /* End of 'Type' function */

        /* Get value function.
         * ARGUMENTS: None.
         * RETURNS:
         *   (Type &) reference to value.
         */
        template<typename Type>
          inline Type & Get( void )
          {
            if constexpr (std::is_same_v<Type, int>)
              return Int;
            if constexpr (std::is_same_v<Type, float>)
              return Float;
            if constexpr (std::is_same_v<Type, bool>)
              return Bool;
            if constexpr (std::is_same_v<Type, vec2>)
              return Vec2;
            if constexpr (std::is_same_v<Type, vec3>)
              return Vec3;
            if constexpr (std::is_same_v<Type, vec4>)
              return Vec4;
          } /* End of 'Get' function */
      }; /* End of 'variable' union */

      // class for mapper variable with name for ui draw system
      class mp_variable
      {
      public:
        variable Variable, OldValue, Min, Max, Default; // mapper variable(value in programm) and his min, max, default, oldvalue.
        BOOL HasChanged {}, IsHasBolder;                // has mapper variable changed
        std::string MapperName;                         // mapper variable name

        /* mp_variable class constructor function.
         * ARGUMENTS:
         *   - mapper variable name:
         *       const std::string &MapName;
         *   - mapper variable value:
         *       const Type &Val;
         */
        template<typename Type>
          mp_variable( const std::string &MapName, const Type &Var, const BOOL &HasBolder, const Type &NewMin, const Type &NewMax, const Type &NewDef ) : MapperName(MapName), Variable(Var), Min(NewMin), Max(NewMax), Default(NewDef), OldValue(Var), IsHasBolder(HasBolder)
          {
          } /* End of 'mp_variable' class constructor function */

        /* Get variable value function.
         * ARGUMENTS: None.
         * RETURNS:
         *   (Type &) reference to variable value.
         */
        template<typename Type>
          inline Type & Get( void )
          {
            if constexpr (std::is_same_v<Type, int>)
              return Variable.Int;
            if constexpr (std::is_same_v<Type, float>)
              return Variable.Float;
            if constexpr (std::is_same_v<Type, bool>)
              return Variable.Bool;
            if constexpr (std::is_same_v<Type, vec2>)
              return Variable.Vec2;
            if constexpr (std::is_same_v<Type, vec3>)
              return Variable.Vec3;
            if constexpr (std::is_same_v<Type, vec4>)
              return Variable.Vec4;
          } /* End of 'Get' function */

        /* Cast to type Type & function.
         * ARGUMENTS: None.
         */
        template <typename Type>
          inline operator Type &( VOID )
          {
            if constexpr(std::is_same_v<Type, int>)
              return Variable.Int;
            else if constexpr(std::is_same_v<Type, float>)
              return Variable.Float;
            else if constexpr(std::is_same_v<Type, bool>)
              return Variable.Bool;
            else if constexpr(std::is_same_v<Type, vec2>)
              return Variable.Vec2;
            else if constexpr(std::is_same_v<Type, vec3>)
              return Variable.Vec3;
            else if constexpr(std::is_same_v<Type, vec4>)
              return Variable.Vec4;
            else
              static_assert(FALSE);
          } /* End of 'Type &' function */

        /* Cast to type Type function.
         * ARGUMENTS: None.
         */
        template<typename Type>
          inline operator Type( VOID )
          {
            if constexpr(std::is_same_v<Type, int>)
              return Variable.Int;
            else if constexpr(std::is_same_v<Type, float>)
              return Variable.Float;
            else if constexpr(std::is_same_v<Type, bool>)
              return Variable.Bool;
            else if constexpr(std::is_same_v<Type, vec2>)
              return Variable.Vec2;
            else if constexpr(std::is_same_v<Type, vec3>)
              return Variable.Vec3;
            else if constexpr(std::is_same_v<Type, vec4>)
              return Variable.Vec4;
            else
              static_assert(FALSE);
          } /* End of 'Type' function */
      }; /* End of 'mp_variable' class */
    } /* End of 'mapper' namespace */
  } /* End of 'ui' namespace */
} /* End of 'tmp' namespace */

#endif /* __tmp_ui_mp_variable_h_ */

/* END OF 'tmp_ui_mp_variable.h' FILE */
