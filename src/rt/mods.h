/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : mods.h
 * PURPOSE     : Raytracing project.
 *               Ray tracing main handle module.
 * PROGRAMMER  : CGSG-SummerCamp'2022.
 *               Andrey Egorov.
 * LAST UPDATE : 25.07.2022.
 * NOTE        : Module namespace 'eart'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mods_h_
#define __mods_h_

#include "def.h"
#include "mth\mth_noise.h"
#include "shapes\shape.h"

namespace eart
{
  class mod_plane_noise_checker : public modifire
  {
    mth::noise *Noise = nullptr;

  public:

    VOID AddNoise( mth::noise *NewNoise )
    {
      Noise = NewNoise;
    }

    VOID Apply( shade_info *Sh ) override
    {

      if (Noise != nullptr)
      {
        //Sh->Surf.Kd = vec3(0.5);
        //Sh->Surf.Ks = vec3(0.5);
        //Sh->Surf.Kd = (Noise->NoiseTurb2D(Sh->P[0] + Sh->P[1], Sh->P[2], 6));


        DBL Color = (Noise->NoiseTurb2D(Sh->P[0], Sh->P[2], 6));
        //Sh->Surf.Kr = (Noise->NoiseTurb2D(Sh->P[0] + Sh->P[1], Sh->P[2], 6)) / 2;

        if ((int(Sh->P[0]) ^ int(Sh->P[2])) & 1)
          Sh->Surf.Kd = Color;
        else
          Sh->Surf.Kd = 1 - Color;

        Sh->Surf.Kr = Color;

        //Sh->Surf.Kd = ((int(Sh->P[0]) ^ int(Sh->P[2])) & 1) ? vec3(0.8) : vec3(0);
        //Sh->Surf.Kr = (Noise->NoiseTurb2D(Sh->P[0] + Sh->P[1], Sh->P[2], 6));
        //Sh->Surf.Kd *= (Noise->NoiseTurb2D(Sh->P[0] + Sh->P[1], Sh->P[2], 6));
        //Sh->Surf.Ks *= (Noise->NoiseTurb2D(Sh->P[0] + Sh->P[1], Sh->P[2], 6));

        //if (Noise->NoiseTurb2D(Sh->P[0] + Sh->P[1], Sh->P[2] + Sh->P[1], 6) > 0.5)
        //  Sh->Surf.Kr = 1;
        //else
        //  Sh->Surf.Kr = 0;
      }
      

    }
  };


  class mod_plane_crooked_checker : public modifire
  {
    mth::noise *Noise = nullptr;

  public:

    VOID AddNoise( mth::noise *NewNoise )
    {
      Noise = NewNoise;
    }

    VOID Apply( shade_info *Sh ) override
    {

      if (Noise != nullptr)
      {
        DBL Coef = 0.8;
        Sh->Surf.Kd = (((INT)floor(Sh->P[0] + Noise->NoiseTurb1D(Sh->P[2], 6) * Coef) ^ (INT)floor(Sh->P[2] + Noise->NoiseTurb1D(Sh->P[0], 6) * Coef)) & 1);
        //Sh->N += vec3(Noise->NoiseTurb1D(Sh->P[2], 6) * Coef, 0, Noise->NoiseTurb1D(Sh->P[0], 6) * Coef);
        //Sh->N.Normalising();
      }

    }
  };

  class mod_plane_rounded_checker : public modifire
  {
    mth::noise *Noise = nullptr;

  public:

    VOID AddNoise( mth::noise *NewNoise )
    {
      Noise = NewNoise;
    }

    VOID Apply( shade_info *Sh ) override
    {

      if (Noise != nullptr)
      {
        Sh->Surf.Kd = (((INT)floor(sqrt(Sh->P[0] * Sh->P[0] + Sh->P[2] * Sh->P[2])) ^ (INT)floor(atan(Sh->P[0] / Sh->P[2]) * 3.14 * 4)) & 1);
        //Sh->N += vec3(Noise->NoiseTurb1D(Sh->P[2], 6) * Coef, 0, Noise->NoiseTurb1D(Sh->P[0], 6) * Coef);
        //Sh->N.Normalising();
      }

    }
  };

  class mod_noise : public modifire
  {
    mth::noise *Noise = nullptr;

  public:

    VOID AddNoise( mth::noise *NewNoise )
    {
      Noise = NewNoise;
    }

    VOID Apply( shade_info *Sh ) override
    {
      if (Noise != nullptr)
      {
        Sh->Surf.Kd = Noise->NoiseTurb3D(Sh->P[0], Sh->P[1], Sh->P[2], 6);
        Sh->Surf.Kr = 0;
      }
    }
  };



  class checker : public modifire
  {
  public:
    VOID Apply( shade_info *Sh ) override
    {
      Sh->Surf.Kd = ((int(Sh->P[0]) ^ int(Sh->P[2])) & 1) ? vec3(0.3) : vec3(0);
    }
  };

  class mod_plane_1: public modifire
  {
  public:
    VOID Apply( shade_info *Sh ) override
    {
      // 1
      Sh->Surf.Kd = ((int(Sh->P[0]) ^ int(Sh->P[2])) & 1) ? vec3(1) : vec3(0);
      
      //Sh->Surf.Ka = vec3(0);
      //
      //Sh->Surf.Ks = vec3(0.01);

      // 2
      //Sh->Surf.Kd = vec3(sin(Sh->P[0]) / 2 + 1, sin(Sh->P[1]) / 2 + 1, sin(Sh->P[2]) / 2 + 1);
      //Sh->Surf.Kd *= ((int(Sh->P[0]) ^ int(Sh->P[2])) & 1);
      
      // 3

      //Sh->Surf.Kd = vec3(sin(Sh->P[0]) / 2 + 1, sin(Sh->P[1]) / 2 + 1, sin(Sh->P[2]) / 2 + 1);
      //Sh->Surf.Kd *= sin(
      //  sqrt(sqrt(sqrt(pow(Sh->P[0] * 8, 8) + pow(Sh->P[2] * 8, 8))))
      //);
      //Sh->Surf.Kd *= ((int(Sh->P[0]) ^ int(Sh->P[2])) & 1) ? 1 : 0;


    }
  };

  

  class rt_checker : public modifire
  {
  public:
    VOID Apply( shade_info *Sh ) override
    {
      Sh->Surf.Kr = ((int(Sh->P[0] * 5) ^ int(Sh->P[2] * 5) ^ int(Sh->P[1] * 5)) & 1) ? 0.5 : 0.3;
      Sh->Surf.Kd = ((int(Sh->P[0] * 5) ^ int(Sh->P[2] * 5) ^ int(Sh->P[1] * 5)) & 1) ? vec3(0, 0, 1) : vec3(0.5, 0, 0);
    }
  };
}

#endif //__mods_h_
