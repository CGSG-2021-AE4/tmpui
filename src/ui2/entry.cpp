#include "entry.h"

/* Update entry global pos function. */
VOID ::ui::entry::UpdateGlobalPos( VOID )
{
  // Don't know is it right
  if (Parent != nullptr)
    GlobalPos = Parent->GlobalPos + LocalPos;
  else if (Canvas != nullptr)
    GlobalPos = Canvas->Pos + LocalPos;
  else
    GlobalPos = LocalPos;

  UpdateMasks();

  for (entry *Child : Children)
    Child->UpdateGlobalPos();
} /* End of 'UpdateGlobalPos' function */