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

/* Redraw entity function - push it to canvas draw manager */
VOID ::ui::entity::Redraw( VOID )
{
#ifdef ENABLE_PATH_LOG
  Log(std::format("Entity {} Redraw.", Id));
#endif // ENABLE_PATH_LOG

  if (!IsVisible)
    return;

  if (IsBackgroundTransparent && Parent != nullptr)
    Parent->Redraw();
  else if (Canvas != nullptr)
    Canvas->PushToDraw(this);
} /* End of 'Redraw' function */