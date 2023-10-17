/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : mth_cam.h
 * PURPOSE     : Raytracing project.
 *               Camera math handle module.
 * PROGRAMMER  : CGSG-SummerCamp'2022.
 *               Andrey Egorov.
 * LAST UPDATE : 23.07.2022.
 * NOTE        : Module namespace 'tmp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_cam_h_
#define __mth_cam_h_

#define MTH_CAM_DEF_UP vec3<FLT>(0, 1, 0)

#define RT_DEF_DIST 1
#define RT_DEF_W_SCR   1
#define RT_DEF_H_SCR   1

#define RT_DEF_WP   1
#define RT_DEF_HP   1


#include "mthdef.h"

namespace mth
{
  template<typename Type>
  class cam
  {
  public:
    vec3<Type> Loc, Dir, Up, Right;
    DBL ProjDist, Wscr, Hscr;
    INT Wp, Hp;

    /* Default construct funtion */
    cam() : Loc(0), Dir(0, 0, 1), Up(MTH_CAM_DEF_UP), Right(Dir % MTH_CAM_DEF_UP), ProjDist(RT_DEF_DIST), Wscr(RT_DEF_W_SCR), Hscr(RT_DEF_H_SCR), Wp(RT_DEF_WP), Hp(RT_DEF_HP)
    {
    }

    /* Construct function with position and direction */
    cam( vec3<Type> NewLoc, vec3<Type> NewDir = vec3<Type>(0) ) : Loc(NewLoc), Dir(NewDir.Normalise), Up(MTH_CAM_DEF_UP), Right(NewLoc % MTH_CAM_DEF_UP), ProjDist(RT_DEF_DIST), Wscr(RT_DEF_W_SCR), Hscr(RT_DEF_H_SCR), Wp(RT_DEF_WP), Hp(RT_DEF_HP)
    {
    }

    void Set( const vec3<Type> &NewLoc, const vec3<Type> &NewDir )
    {
      Loc = NewLoc;
      Dir = NewDir.Normalise();
      Right = (Dir % Up);
      Right.Normalising();
      //Up = VecCrossVec(Right, Dir);
    }
    void Rotate( vec3<DBL> LocalRotation )
    {
      matr<DBL> RotationM = matr<DBL>::Rotate(LocalRotation[0], Right) * matr<DBL>::Rotate(LocalRotation[1], Up) * matr<DBL>::Rotate(LocalRotation[2], Dir);
      Set(Loc, RotationM.TransformVector(Dir));
    }
    void Move( mth::vec3<DBL> LocalMove )
    {
      Loc +=  Right * LocalMove[0] + Up * LocalMove[1] + Dir * LocalMove[2];
    }

    void Resize( const INT &NewWp, const INT &NewHp )
    {
      Wp = NewWp;
      Hp = NewHp;
    }

    mth::ray<DBL> FrameToRay( const INT &x, const INT &y, const INT &Smooth = 1)
    {
      mth::ray<DBL> Out;

      Out.Org = Loc;
      Out.Dir = Up * ((DBL)-y / (Hp * Smooth) + 0.5) * Hscr +
            Right * ((DBL)x / (Wp * Smooth) - 0.5) * Hscr * Wp / Hp +
            Dir * ProjDist;
      Out.Dir = Out.Dir.Normalise();
      return Out;
    }
  };
}

#endif //__mth_cam_h_
