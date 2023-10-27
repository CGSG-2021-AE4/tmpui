#include "entity.h"
#include "canvas.h"

/* Update entity global pos function. */
VOID ::ui::entity::UpdateGlobalPos( VOID )
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