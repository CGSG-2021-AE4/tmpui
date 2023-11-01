#include <algorithm>

#include "ui_def.h"

#ifndef __entity_h_
#define __entity_h_

#include "props.h"

//#define ENABLE_PATH_LOG

// Some real kal
using namespace tmp;

namespace ui
{
  /* Friend classes prototypes */
  class canvas;

  /* Physical entity state */
  enum struct entity_state
  {
    eDef     = 0,
    eHovered = 1,
    eFocused = 2,
    eActive  = 3,
  }; /* End of 'entity_state' enum struct */

  /* UI entity class */
  class entity
  {
    /* Values */
  protected:

    // Info

    std::string Id {""};                   // Entries id

    // Position

    ivec2
      LocalPos {0},                        // Position in parent basis
      GlobalPos {0},                       // Position in canvas basis
      GlobalContentPos {0},                // Position in canvas basis
      ContentOffset {0};                   // Content offset

    // Sizes

    isize2
      Size {0},                            // Size
      BorderSize {0},                      // Border size
      ContentSize {0},                     // Content size
      CompContentSize {0},                 // Computed content size
      MarginSize {0};                      // Margin size

    // Masks

    mask
      SelfMask,                            // Self mask
      ContentMask,                         // Content mask
      SelfDrawMask,                        // Mask that is visible on the screen
      ContentDrawMask;                     // Mask for content that is visible on the screen
    
    // Neighbours pointers

    entity *Parent {nullptr};               // Parent pointer
    canvas *Canvas {nullptr};              // Canvas pointer
    std::vector<entity *> Children;         // Children pointers

    // Dynamic state

    entity_state
      State{ entity_state::eDef };          // State of entity

    // Style stuff

    BOOL IsVisible;                        // Is entity visible

    layout_props LayoutProps {};           // Props of entity's layout
    box_props BoxProps {};                 // Props of entity's box
    FLT ChildrenFlexSum = 0;               // Sum of children's flex values (is used only with FlexRow/FlexColumn flex type)
    isize2 ChildrenFlex0Size = 0;          // Sum of min sizes of children with flex 0
    BOOL IsScrollable = 0;                 // Can the entity be scrolled
    BOOL IsBackgroundTransparent = 0;      // If the background is transparent entity's parent should also be redrawn 

    /* Events */
  public:

    /* Set visibility function */
    VOID SetVisibility( BOOL NewValue )
    {
      IsVisible = NewValue;
    } /* End of 'SetVisibility' function */

    /* Is point over entity function */
    BOOL IsOver( const ivec2 &GlobalPoint )
    {
      return SelfMask(GlobalPoint);
    } /* End of 'IsOver' function */

    /******* Virtual event functions for overriding by user *******/
    
    /* User draw event function - only paints */
    virtual VOID OnDraw( VOID )
    {
    } /* End of 'OnDraw' function */

    /* Other user event functions return BOOL - should the entity be redrawn */

    virtual BOOL OnHover( const ivec2 &LocalMousePos )
    {
      return false;
    } /* End of 'OnHover' function */

    virtual BOOL OnUnhover( const ivec2 &LocalMousePos )
    {
      return false;
    } /* End of 'OnUnhover' function */

    virtual BOOL OnClick( const ivec2 &LocalMousePos )
    {
      return false;
    } /* End of 'OnClick' function */

    virtual BOOL OnMouseDown( const ivec2 &LocalMousePos )
    {
      return false;
    } /* End of 'OnMouseDown' function */

    virtual BOOL OnMouseUp( const ivec2 &LocalMousePos )
    {
      return false;
    } /* End of 'OnMouseUp' function */

    virtual BOOL OnMouseMove( const ivec3 &Delta, const ivec2 &LocalMousePos )
    {
      return false;
    } /* End of 'OnMouseMove' function */

    virtual BOOL OnDrag( const ivec3 &Delta, const ivec2 &LocalMousePos )
    {
      return false;
    } /* End of 'OnDrag' function */

    virtual BOOL OnChange( VOID )
    {
      return false;
    } /* End of 'OnChange' function */

    virtual VOID OnResize( VOID )
    {
    } /* End of 'OnResize' function */

    virtual VOID OnMove( VOID )
    {
    } /* End of 'OnResize' function */

    /* User mask functions */

    virtual mask GetSelfMask( const ivec2 &GlobalPos, const isize2 &Size )
    {
      return {GlobalPos, Size};
    } /* End of 'GetSelfMask' function */

    virtual mask GetContentMask( const ivec2 &GlobalContentPos, const isize2 &ContentSize )
    {
      return {GlobalContentPos, ContentSize};
    } /* End of 'GetContentMask' function */

    virtual isize2 GetMinSize( const isize2 &PreferedtSize )
    {
      return LayoutProps.MinSize;
    } /* End of 'GetMinSize' function */

    /* ****** Inline event functions (interlayer) ******
     * Functions return (entity *) - pointer to entity to redraw or nullptr. It could be the upper level entity because of event bubbling. The pointer to entity may be used later for draw.
     */

    inline entity * OnHoverEvent( const ivec2 &LocalMousePos )
    {
      //Log(std::format("Entity {} OnHover event.", Id));
      if (State == entity_state::eDef)
        State = entity_state::eHovered;
      return OnHover(LocalMousePos) ? this : nullptr;
    } /* End of 'OnHoverEvent' function */

    inline entity * OnUnhoverEvent( const ivec2 &LocalMousePos )
    {
      //Log(std::format("Entity {} OnUnhover event.", Id));
      if (State == entity_state::eHovered)
        State = entity_state::eDef;
      return OnUnhover(LocalMousePos) ? this : nullptr;
    } /* End of 'OnUnhoverEvent' function */

    inline entity * OnClickEvent( const ivec2 &LocalMousePos )
    {
      Log(std::format("Entity {} OnClick event.", Id));
      return OnClick(LocalMousePos) ? this : Parent != nullptr ? Parent->OnClickEvent(LocalMousePos + LocalPos) : nullptr;
    } /* End of 'OnClickEvent' function */

    inline entity * OnMouseDownEvent( const ivec2 &LocalMousePos )
    {
      //Log(std::format("Entity {} OnMouseDown event.", Id));
      State = entity_state::eActive;
      return OnMouseDown(LocalMousePos) ? this : nullptr;
    } /* End of 'OnMouseDownEvent' function */

    inline entity * OnMouseUpEvent( const ivec2 &LocalMousePos )
    {
      //Log(std::format("Entity {} OnMouseUp event.", Id));
      State = entity_state::eHovered;
      return OnMouseUp(LocalMousePos) ? this : nullptr;
    } /* End of 'OnMouseUpEvent' function */

    inline entity * OnMouseMoveEvent( const ivec3 &Delta, const ivec2 &LocalMousePos )
    {
      //Log(std::format("${} OnMouseMove event.", Id));
      if (LayoutProps.IsScrollable && Delta.Z != 0) // May be later I'l add a check with the content mask
      {
        // Scroll response
        ivec2 DeltaMove = {0, Delta.Z}; // Some temp shit

        // Clamping
        auto OldContentOffset = ContentOffset;

        ContentOffset = {std::clamp(ContentOffset.X + DeltaMove.X, ContentSize.W - CompContentSize.W, 0),
                         std::clamp(ContentOffset.Y + DeltaMove.Y, ContentSize.H - CompContentSize.H, 0)};
        
        if (OldContentOffset == ContentOffset)
          return nullptr;
        
        for (auto *c : Children)
        {
          c->UpdateGlobalPosRec();
          c->UpdateMasksRec();
        }
        return this;
      }

      return OnMouseMove(Delta, LocalMousePos) ? this : Parent != nullptr ? Parent->OnMouseMoveEvent(Delta, LocalMousePos + LocalPos) : nullptr;
    } /* End of 'OnMouseMoveEvent' function */

    inline entity * OnDragEvent( const ivec3 &Delta, const ivec2 &LocalMousePos )
    {
      //Log(std::format("Entity {} OnDrag event.", Id));
      return OnDrag(Delta, LocalMousePos) ? this : Parent != nullptr ? Parent->OnDragEvent(Delta, LocalMousePos + LocalPos) : nullptr;
    } /* End of 'OnDragEvent' function */

    inline entity * OnChangeEvent( VOID )
    {
      //Log(std::format("Entity {} OnChange event.", Id));
      return OnChange() ? this : nullptr;
    } /* End of 'OnChangeEvent' function */

    inline VOID OnResizeEvent( VOID )
    {
      //Log(std::format("Entity {} OnResize event.", Id));
      OnResize();
    } /* End of 'OnResizeEvent' function */

    inline VOID OnMoveEvent( VOID )
    {
      //Log(std::format("Entity {} OnMove event.", Id));
      OnMove();
    } /* End of 'OnResizeEvent' function */
    
  protected:
  public: // !!! TMP DEBUG SHIT

    friend class canvas;

    /* Entity with id mentioned constructor function.
     * ARGUMENTS:
     *   - id string:
     *       const std::string &NewId;
     *   - pos:
     *       const ivec2 &NewPos;
     *   - size:
     *       const isize2 &NewSize;
     *   - props:
     *       const props_type &Props;
     *   - children:
     *       const std::vector<entity *> &NewChildren;
     *   - parent:
     *       entity *NewParent;
     */
    template<typename props_type>
      entity( const props_type &Props, const std::vector<entity *> &NewChildren = {}, entity *NewParent = nullptr ) :
        Parent(NewParent),
        IsVisible(true)
      {
#ifdef ENABLE_PATH_LOG
        Log(std::format("Entity {} constructor.", Id));
#endif // ENABLE_PATH_LOG

        // Setup info from props

        if constexpr (requires { Props.Id; })
          Id = Props.Id;
        if constexpr (requires { Props.LayoutProps; })
          LayoutProps = Props.LayoutProps;
        if constexpr (requires { Props.BoxProps; })
          BoxProps = Props.BoxProps;
        if constexpr (requires { Props.Pos; })
          SetPos(Props.Pos);
        if constexpr (requires { Props.Size; })
          SetSize(Props.Size);
        
        OnMove();
        OnResize();

        SetParent(NewParent);
        AddChildren(NewChildren);
        UpdateShape();
      } /* End of 'entity' function */

  protected:
    /* Entity desctrictor function */
    virtual ~entity( VOID )
    {
//#ifdef ENABLE_PATH_LOG
      Log(std::format("Entity {} Destructor.", Id));
//#endif // ENABLE_PATH_LOG

      // SOME REAL SHIT
      for (entity *c : Children)
        delete c;
      Children.clear();
    } /* End of '~entity' function */

    /* Update min size function */
    // const isize2 & UpdateMinSize( const isize2 &PreferedSize )
    // {
    //   if (LayoutProps.Type == layout_type::eFlexRow)
    //   {
    //     LayoutProps.MinSize = 0;
    //     for (entity *c : Children)
    //     {
    //       const isize2 &ChildMinSize = c->UpdateMinSize({0, PreferedSize.H});
    //       LayoutProps.MinSize.W += ChildMinSize.W;
    //       LayoutProps.MinSize.H = std::max(LayoutProps.MinSize.H, ChildMinSize.H);
    //     }
    //   }
    //   else if (LayoutProps.Type == layout_type::eFlexColumn)
    //   {
    //     LayoutProps.MinSize = 0;
    //     for (entity *c : Children)
    //     {
    //       const isize2 &ChildMinSize = c->UpdateMinSize({PreferedSize.W, 0});
    //       LayoutProps.MinSize.W = std::max(LayoutProps.MinSize.W, ChildMinSize.W);
    //       LayoutProps.MinSize.H += ChildMinSize.H;
    //     }
    //   }
    //   else if (LayoutProps.Type == layout_type::eBlock)
    //   {
    //     LayoutProps.MinSize = GetMinSize(PreferedSize);
    //   }
    // } /* End of 'UpdateMinSize' function */
    // 
    public:
    /* Update children layout function */
    VOID UpdateChildrenLayout( VOID )
    {
      if (LayoutProps.Type == layout_type::eFlexRow)
      {
        INT ContentFlexSizeW = ContentSize.W - ChildrenFlex0Size.W;

        CompContentSize = {};
        if (ChildrenFlexSum == 0)
        {
          INT Offset = 0;
          isize2 NewSize;

          for (entity *c : Children)
          {
            NewSize = Clamp({c->LayoutProps.MinSize.W, ContentSize.H}, c->LayoutProps.MinSize, c->LayoutProps.MaxSize);
            c->Reform({Offset, 0}, NewSize);
            Offset += NewSize.W;
            CompContentSize.W += NewSize.W;
            CompContentSize.H = std::max(CompContentSize.H, NewSize.H);
          }
        }
        else
        {
          INT Offset = 0;
          isize2 NewSize;

          for (entity *c : Children)
          {
            NewSize = Clamp({(INT)(c->LayoutProps.Flex / ChildrenFlexSum * ContentFlexSizeW), ContentSize.H}, c->LayoutProps.MinSize, c->LayoutProps.MaxSize);
            c->Reform({Offset, 0}, NewSize);
            Offset += NewSize.W;
            CompContentSize.W += NewSize.W;
            CompContentSize.H = std::max(CompContentSize.H, NewSize.H);
          }
        }
      }
      else if (LayoutProps.Type == layout_type::eFlexColumn)
      {
        CompContentSize = {};

        INT ContentFlexSizeH = ContentSize.H - ChildrenFlex0Size.H;
        if (ChildrenFlexSum == 0)
        {
          INT Offset = 0;
          isize2 NewSize;

          for (entity *c : Children)
          {
            NewSize = Clamp({ContentSize.W, c->LayoutProps.MinSize.H}, c->LayoutProps.MinSize, c->LayoutProps.MaxSize);
            c->Reform({0, Offset}, NewSize);
            Offset += NewSize.H;
            CompContentSize.H += NewSize.H;
            CompContentSize.W = std::max(CompContentSize.W, NewSize.W);
          }
        }
        else
        {
          INT Offset = 0;
          isize2 NewSize;


          for (entity *c : Children)
          {
            NewSize = Clamp({ContentSize.W, (INT)(c->LayoutProps.Flex / ChildrenFlexSum * ContentFlexSizeH)}, c->LayoutProps.MinSize, c->LayoutProps.MaxSize);
            c->Reform({0, Offset}, NewSize);
            Offset += NewSize.H;
            CompContentSize.H += NewSize.H;
            CompContentSize.W = std::max(CompContentSize.W, NewSize.W);
          }
        }
      }
      else
        for (entity *c : Children)
          c->UpdateShape();

      if (IsScrollable)
        ContentOffset = {std::clamp(ContentOffset.X, ContentSize.W - CompContentSize.W, 0),
                         std::clamp(ContentOffset.Y, ContentSize.H - CompContentSize.H, 0)};
      // if (LayoutProps.Type == layout_type::eBlock)
      // return;
    } /* End of 'UpdateChildrenLayout' function */

    /* Update self and content masks function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID UpdateMasks( VOID )
    {
      SelfMask = GetSelfMask(GlobalPos, Size);
      ContentMask = GetContentMask(GlobalPos + ivec2(BoxProps.BorderW), BorderSize);
      if (Parent != nullptr)
      {
        SelfDrawMask = SelfMask.Intersect(Parent->ContentDrawMask); // May be later add check for mask change
        ContentDrawMask = ContentMask.Intersect(SelfDrawMask);
      }
      else
      {
        SelfDrawMask = SelfMask;
        ContentDrawMask = ContentMask.Intersect(SelfDrawMask);
      }
    } /* End of 'UpdateDrawMask' function */

    /* Update self and childrens masks function (recursive) */
    VOID UpdateMasksRec( VOID )
    {
      UpdateMasks();
      for (entity *c : Children)
        c->UpdateMasksRec();
    } /* End of 'UpdateMasksRec' function */

    /* Update entity global pos function */
    VOID UpdateGlobalPos( VOID );

    /* Update entity global pos recursive function (+ update children) */
    VOID UpdateGlobalPosRec( VOID )
    {
      UpdateGlobalPos();
      for (entity *c : Children)
        c->UpdateGlobalPosRec();
    } /* End of 'UpdateGlobalPos' function */

    /* Update shape function */
    VOID UpdateShape( VOID )
    {
#ifdef ENABLE_PATH_LOG
      Log(std::format("Entity {} update shape.", Id));
#endif // ENABLE_PATH_LOG

      UpdateGlobalPos();
      UpdateMasks();
      UpdateChildrenLayout();
    } /* End of 'UpdateShape' function */

    /* Set position function */
    inline VOID SetPos( const ivec2 &NewLocalPos )
    {
      LocalPos = NewLocalPos + ivec2(BoxProps.MarginW);
    } /* End of 'SetPos' function */

    /* Set size function */
    inline VOID SetSize( const isize2 &NewMarginSize )
    {
      MarginSize = NewMarginSize;
      Size = MarginSize - isize2(BoxProps.MarginW) * 2;
      BorderSize = Size - isize2(BoxProps.BorderW) * 2;
      ContentSize = BorderSize - isize2(BoxProps.PaddingW) * 2;
    } /* End of 'SetSize' function */

  public:

    /* Resize function.
     * ARGUMENTS:
     *   - new size:
     *       const isize2 &NewSize;
     * RETURNS: None.
     */
    VOID Resize( const isize2 &NewSize )
    {
#ifdef ENABLE_PATH_LOG
      Log(std::format("Entity {} Resize.", Id));
#endif // ENABLE_PATH_LOG

      // Update sizes
      SetSize(NewSize);
      
      UpdateShape();
      OnResize();
    } /* End of 'Resize' function */

    /* Move function.
     * ARGUMENTS:
     *   - new local pos:
     *       const ivec2 &NewLocalPos;
     * RETURNS: None.
     */
    VOID Move( const ivec2 &NewLocalPos )
    {
#ifdef ENABLE_PATH_LOG
      Log(std::format("Entity {} Move.", Id));
#endif // ENABLE_PATH_LOG
      SetPos(NewLocalPos);

      UpdateGlobalPosRec();
      UpdateMasksRec();
      OnMove();
    } /* End of 'Resize' function */

    /* Combined move and resize function.
     * ARGUMENTS:
     *   - new local pos:
     *       const ivec2 &NewLocalPos;
     *   - new size:
     *       const isize2 &NewSize;
     * RETURNS: None.
     */
    VOID Reform( const ivec2 &NewLocalPos, const isize2 &NewSize )
    {
#ifdef ENABLE_PATH_LOG
      Log(std::format("Entity {} Reform.", Id));
#endif // ENABLE_PATH_LOG
      SetPos(NewLocalPos);
      SetSize(NewSize);

      UpdateShape();
      OnMove();
      OnResize();
    } /* End of 'Reform' function */

    /* Draw entity's children function */
    VOID DrawChildren( VOID )
    {
      for (entity *c : Children)
        c->Draw();
    } /* End of 'DrawChildren' function */

    /* Draw entity function */
    VOID Draw( VOID )
    {
#ifdef ENABLE_PATH_LOG
      Log(std::format("Entity {} Draw.", Id));
#endif // ENABLE_PATH_LOG
      if (!IsVisible || !SelfDrawMask.IsExist())
        return;

      OnDraw();
      DrawChildren();
    } /* End of 'Draw' function */

    /* Redraw entity function - push it to canvas draw manager */
    VOID Redraw( VOID );

    /************ Parents/Children operations functions ************/

    /* Set entity's parent function.
     * ARGUMENTS:
     *   - new parent:
     *       entity *NewParent;
     * RETURNS: None.
     */
    VOID SetParent( entity *NewParent )
    {
#ifdef ENABLE_PATH_LOG
      Log(std::format("Entity {} SetParent.", Id));
#endif // ENABLE_PATH_LOG
      // Rebind to parent
      if (Parent != nullptr)
        Parent->DeleteChild(this); // Delete self from parent's children

      Parent = NewParent;

      if (Parent != nullptr)
        Parent->AddChild(this); // Add self to new parent's children
      
    } /* End of 'SetParent' function */

    /* On add entity as a child to one parent function.
     * ARGUMENTS:
     *   - new entity's parent:
     *       entity *NewParent;
     * RETURNS: None.
     */
    VOID OnAddChild( entity *NewParent )
    {
#ifdef ENABLE_PATH_LOG
      Log(std::format("Entity {} OnAddChild.", Id));
#endif // ENABLE_PATH_LOG
      // Rebind to parent
      if (Parent != nullptr)
        Parent->DeleteChild(this); // Delete self from parent's children

      Parent = NewParent;

      if (Parent != nullptr)
        SetCanvas(Parent->Canvas);

      UpdateShape();
    } /* End of 'OnAddChild' function */

    /* Set canvas function.
     * ARGUMENTS:
     *   - new canvas:
     *       canvas *NewCanvas;
     * RETURNS: None.
     */
    VOID SetCanvas( canvas *NewCanvas )
    {
      Canvas = NewCanvas;
      for (entity *Child : Children)
        Child->SetCanvas(Canvas);
    } /* End of 'SetParent' function */

    // Later there must be funtions to instert children into specific positions

  private:

    /* Add child private function.
     * ARGUMENTS:
     *   - adding child:
     *       entity *NewChild;
     * RETURNS: None.
     */
    VOID AddChildUnsafe( entity *NewChild )
    {
      if (NewChild == nullptr)
        return;

      for (entity *Child : Children)
        if (NewChild == Child)
          return; // We already have this child
    
      Children.push_back(NewChild);
      NewChild->OnAddChild(this);
      ChildrenFlexSum += NewChild->LayoutProps.Flex;
      if (NewChild->LayoutProps.Flex == 0)
        ChildrenFlex0Size += NewChild->LayoutProps.MinSize;
    } /* End of 'AddChildUnsafe' function */

  public:

    /* Add child public function.
     * ARGUMENTS:
     *   - adding child:
     *       entity *NewChild;
     * RETURNS: None.
     */
    VOID AddChild( entity *NewChild )
    {
      AddChildUnsafe(NewChild);
      UpdateChildrenLayout();
    } /* End of 'AddChild' function */

    /* Add children function.
     * ARGUMENTS:
     *   - adding children:
     *       const std::vector<entity *> &NewChildren;
     * RETURNS: None.
     */
    VOID AddChildren( const std::vector<entity *> &NewChildren )
    {
      for (entity *Child : NewChildren)
        AddChild(Child);

      UpdateChildrenLayout();
    } /* End of 'AddChildren' function */

    /* Find a child function.
     * ARGUMENTS:
     *   - child's pointer:
     *       const entity *Child;
     * RETURNS:
     *   (std::vector<entity *>::iterator) - iterator to the child or end().
     */
    std::vector<entity *>::iterator FindChild( const entity *Child )
    {
      for (std::vector<entity *>::iterator c = Children.begin(); c != Children.end(); ++c)
        if (Child == *c)
          return c;

      return Children.end();
    } /* End of 'FindChild' function */

    /* Delete child function.
     * ARGUMENTS:
     *   - child's pointer:
     *       const entity *Child;
     * RETURNS: None.
     */
    VOID DeleteChild( entity *Child )
    {
      const std::vector<entity *>::iterator FoundChild = FindChild(Child);

      if (FoundChild == Children.end())
        return;
    
      ChildrenFlexSum -= (*FoundChild)->LayoutProps.Flex;
      if ((*FoundChild)->LayoutProps.Flex == 0)
        ChildrenFlex0Size -= (*FoundChild)->LayoutProps.MinSize;
      Children.erase(FoundChild);
      UpdateChildrenLayout();
    } /* End of 'DeleteChild' function */

  }; /* End of 'entity' class */

} /* End of 'ui' namespace */

#endif // __entity_h_