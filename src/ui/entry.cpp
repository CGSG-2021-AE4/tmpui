#include "entry.h"
#include "canvas.h"

/* Update entry global pos function. */
VOID ::ui::entry::UpdateGlobalPos( VOID )
{
  // Don't know is it right
  if (Parent != nullptr)
    GlobalPos = Parent->GlobalContentPos + Parent->ContentOffset + LocalPos;
  else if (Canvas != nullptr)
    GlobalPos = Canvas->Pos + LocalPos;
  else
    GlobalPos = LocalPos;

  GlobalContentPos = GlobalPos + ivec2(BoxProps.BorderW + BoxProps.PaddingW);
} /* End of 'UpdateGlobalPos' function */