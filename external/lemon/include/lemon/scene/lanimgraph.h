//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lanimgraph.h
// Description : Animation Graph system include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LANIMGRAPH_H_INCLUDED
#define LANIMGRAPH_H_INCLUDED

#include "./lsceneapi.h"
#include "./lanim.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lAnimGraphManager
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lAnimGraphSourceBase;
class lAnimGraphOutput;
class lAnimGraphNode;

class LSCENE_API lAnimGraphManager
{
public:
  lAnimGraphManager();
  ~lAnimGraphManager();

  //##
  // Create lAnimGraphNodeSource from animation node hierarchy
  lAnimGraphSourceBase *CreateSource( lAnimNode *root, const char *sourceName );

  //##
  // Create lAnimGraphNodeSource from source + segment information
  lAnimGraphSourceBase *CreateSegmentedSource( lAnimGraphSourceBase *sourceBase, const char *segmentName, float tStart, float tEnd, bool bLoop, float tScale = 1.0f );

  //##
  // Create lAnimGraphOutput for an animation node hierarchy
  lAnimGraphOutput *CreateOutput( lAnimNode *root, lAnimGraphNode *graphOutputNode );

  ////##
  //// Duplicate a graph node hierarchy
  //lAnimGraphNode *DuplicateHierarchy( lAnimGraphNode *rootToCopy );

  //##
  // Lookup animation source
  lAnimGraphSourceBase *LookupSource( const char *sourceName );

  //##
  // Lookup animation node index within source
  int  LookupNodeIndex( const char *sourceName, const char *nodeName );
  int  LookupNodeIndex( lAnimGraphSourceBase *source, const char *nodeName );

protected:
  friend class lAnimGraphSourceBase;

  void RemoveSource( lAnimGraphSourceBase *source );

private:
  struct sourceInfo
  {
    lAnimGraphSourceBase *source;
    lAnimNode            *hierarchyRoot;
  };

  lArray<sourceInfo,TSceneArrayTrait> m_sourceList;
};


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Source classes
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

// this provides a source interface for animation data
class LSCENE_API lAnimGraphSourceBase
{
public:
  lAnimGraphSourceBase();
  virtual ~lAnimGraphSourceBase();

  // lAnimGraphSourceBase interface
  virtual float           GetLength() = 0;
  virtual int             GetNumNodes() const = 0;
  virtual void            GetXform( float t, int animNodeIndex, lAnimXform &xform ) = 0;
  virtual const char    * GetName()
  {
    return m_name.CStr();
  }

protected:
  friend class lAnimGraphManager;

  lString             m_name;
  lAnimGraphManager  *m_manager;
};

// this simply provides a source interface for animation data
class LSCENE_API lAnimGraphSource : public lAnimGraphSourceBase
{
public:
  lAnimGraphSource();
  virtual ~lAnimGraphSource();

  // lAnimGraphSourceBase interface
  virtual float           GetLength()
  {
    return m_length;
  }

  virtual int             GetNumNodes() const
  {
    return m_nodeList.Size();
  }

  virtual void            GetXform( float t, int animNodeIndex, lAnimXform &xform );

protected:
  bool Init( lAnimNode *root, const char *name, lAnimGraphManager *manager );

  friend class lAnimGraphManager;

  struct nodeInfo
  {
    lString           sourceNodeName;
    lAnimNode        *sourceNode;
    lAnimation       *sourceAnimation;
    int               parentIdx;
  };

  lArray<nodeInfo,TSceneArrayTrait>    m_nodeList;
  float               m_length;
};

// this segments an lAnimGraphSource into a seperate section for splitting a single animation into multiples
class LSCENE_API lAnimGraphSourceSegment : public lAnimGraphSourceBase
{
public:
  lAnimGraphSourceSegment();
  virtual ~lAnimGraphSourceSegment();

  // lAnimGraphSourceBase

  virtual float           GetLength();
  virtual int             GetNumNodes() const;
  virtual void            GetXform( float t, int animNodeIndex, lAnimXform &xform );

protected:
  bool Init( lAnimGraphSourceBase *source, const char *name, float tStart, float tEnd, float tScale, bool bLoop, lAnimGraphManager *manager );

  friend class lAnimGraphManager;

  lAnimGraphSourceBase *m_base;
  float                 m_startT;
  float                 m_endT;
  float                 m_seglength;
  float                 m_seglengthbase;
  float                 m_scaleT;
  bool                  m_bLoop;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Output classes
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

// this simply provides an output interface for animation data
class LSCENE_API lAnimGraphOutput
{
public:
  lAnimGraphOutput();
  virtual ~lAnimGraphOutput();

  // evaluate the graph for the current time
  virtual void              Evaluate();

  virtual lAnimGraphNode  * GetGraphOutputNode();
  virtual lAnimNode       * GetAnimNode();

  //TEMP
  lmMatrix4                 GetLocalMatrix( int i )
  {
    return m_animNodeArray(i)->m_localMatrix;
  }
  //TEMP

protected:
  bool Init( lAnimNode *root, lAnimGraphNode *graphOutputNode, lAnimGraphManager *manager );

  friend class lAnimGraphManager;

private:
  lAnimGraphNode     *m_graphNode;
  lAnimNode          *m_animNode;
  lAnimGraphManager  *m_manager;

  lArray<lAnimNode *,TSceneArrayTrait> m_animNodeArray;
};


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Graph Node classes
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

// this is the base class for the graph system
class LSCENE_API lAnimGraphNode
{
protected:
  lAnimGraphNode();

public:
  virtual ~lAnimGraphNode();

  void            AdvanceTime( float deltaT );
  float           GetTime() const;
  virtual void    SetTime( float t );

  //##
  // Return the length of the animation represented by this node in seconds
  // may return LM_FLOAT_MAX if the animation can run infinitely (i.e. looped or generated)
  // NOTE: The returned length is the maximum length of the node with it's current settings.
  virtual float   GetLength() = 0;

  virtual void    AdvanceHierarchyTime( float deltaT ) = 0;

  virtual int     GetNumNodes() const = 0;
  virtual void    ComputeXform( int animNodeIndex, lAnimXform &xform ) = 0;

protected:
  float     m_currentTime;
};


// this class links up a lAnimGraphSource with playback direction / speed / looping
class LSCENE_API lAnimGraphNodeSource : public lAnimGraphNode
{
 public:
  // playmode + direction

  enum PlayMode
  {
    PLAY_ONCE,
    PLAY_LOOPED,
    PLAY_PINGPONG
  };

  enum Direction
  {
    DIR_FORWARD,
    DIR_BACKWARD
  };


protected:
  // constructor
  lAnimGraphNodeSource( lAnimGraphSourceBase *anim = NULL, PlayMode mode = PLAY_LOOPED, Direction dir = DIR_FORWARD );

public:
  virtual ~lAnimGraphNodeSource();

  // factory methods
  static lAnimGraphNodeSource *Create();
  static lAnimGraphNodeSource *Create( lAnimGraphSourceBase *anim, PlayMode mode = PLAY_LOOPED, Direction dir = DIR_FORWARD );

  // lAnimGraphNode methods
  virtual void      SetTime( float t );
  virtual void      AdvanceHierarchyTime( float deltaT );

  virtual int       GetNumNodes() const;
  virtual void      ComputeXform( int animNodeIndex, lAnimXform &xform );

  virtual float     GetLength();

  // lAnimGraphNodeSource methods
  void              SetPlayMode( PlayMode mode );

  void              Start();
  void              Stop();
  void              ChangeDirection(Direction dir);
  void              FlipDirection();
  void              SetSpeed(float speed);
  float             GetSpeed() const;

  float             GetAnimationLength() const;
  float             GetAdjustedLength() const;
  float             GetAnimTime() const;

private:
  float             ApplyPlayMode(float t, PlayMode mode) const;
  float             ApplyDirection(float t, Direction dir) const;

  static float      AnimLoop(float x, float length);
  static float      AnimPingPong(float x, float length);

  static Direction  FlipDirection(Direction dir);

  // variables
  lAnimGraphSourceBase *m_anim;
  float                 m_animTime;
  PlayMode              m_mode;
  Direction             m_direction;
  float                 m_playingTime;
  float                 m_startTime;
  float                 m_tOffset;
  float                 m_length;
  float                 m_speed;
  bool                  m_bPlaying;
};


// this class links up two lAnimGraphNode objects
class LSCENE_API lAnimGraphNodeCombine : public lAnimGraphNode
{
protected:
  lAnimGraphNodeCombine( lAnimGraphNode *input0 = NULL, lAnimGraphNode *input1 = NULL );

public:
  virtual ~lAnimGraphNodeCombine();

  // lAnimGraphNode methods
  virtual void            SetTime(float time);
  virtual void            AdvanceHierarchyTime( float deltaT );
  virtual int             GetNumNodes() const;
  virtual float           GetLength();

  // lAnimGraphNodeCombined methods
  virtual void            ReplaceInput( lAnimGraphNode *child, lAnimGraphNode *with );
  virtual bool            IsInput0Active() const;
  virtual bool            IsInput1Active() const;

  lAnimGraphNode        * GetInput0();
  lAnimGraphNode        * GetInput1();

  void                    SetInput0( lAnimGraphNode *source );
  void                    SetInput1( lAnimGraphNode *source );
  void                    SetInputs( lAnimGraphNode *source0, lAnimGraphNode *source1 );
protected:
  lAnimGraphNode *m_input0;
  lAnimGraphNode *m_input1;
};


// this class blends two lAnimGraphNode objects
class LSCENE_API lAnimGraphNodeBlend : public lAnimGraphNodeCombine
{
protected:
  lAnimGraphNodeBlend( lAnimGraphNode *input0 = NULL, lAnimGraphNode *input1 = NULL );

public:
  virtual ~lAnimGraphNodeBlend();

  // factory methods
  static lAnimGraphNodeBlend *Create( lAnimGraphNode *input0, lAnimGraphNode *input1 );

  // lAnimNode methods
  virtual void                ComputeXform( int animNodeIndex, lAnimXform &xform );

  // lAnimGraphNodeCombined methods
  virtual bool                IsInput0Active() const;
  virtual bool                IsInput1Active() const;

  // lAnimGraphNodeBlend methods
  void                        SetBlendFactor( float blendFactor );
  float                       GetBlendFactor() const;

private:
  float m_blendFactor;
};


// this class mixes two lAnimGraphNode objects
class LSCENE_API lAnimGraphNodeMix : public lAnimGraphNodeCombine
{
protected:
  lAnimGraphNodeMix( lAnimGraphNode *input0 = NULL, lAnimGraphNode *input1 = NULL );
  lAnimGraphNodeMix( lAnimGraphNode *input0, lAnimGraphNode *input1, const lArray<int> &input1Nodes );

public:
  virtual ~lAnimGraphNodeMix();

  // factory methods
  static lAnimGraphNodeMix *Create( lAnimGraphNode *input0 = NULL, lAnimGraphNode *input1 = NULL );
  static lAnimGraphNodeMix *Create( lAnimGraphNode *input0, lAnimGraphNode *input1, const lArray<int> &input1Nodes );

  // lAnimNode methods
  virtual void              ComputeXform( int animNodeIndex, lAnimXform &xform );

  // lAnimGraphNodeMix methods
  void                      SetNodeSource(int animNodeIndex, int source);

private:
  void InitNodeMix();

  // variables
  lArray<int,TSceneArrayTrait> m_nodeMix;
};

// animation timed blend node
class LSCENE_API lAnimGraphNodeTimedBlend : public lAnimGraphNodeBlend
{
protected:
  lAnimGraphNodeTimedBlend();
  lAnimGraphNodeTimedBlend( lAnimGraphNode *input0, lAnimGraphNode *input1, float blendStartTime, float blendTimePeriod );

public:
  virtual ~lAnimGraphNodeTimedBlend();

  // factory methods
  static lAnimGraphNodeTimedBlend *Create();
  static lAnimGraphNodeTimedBlend *Create( lAnimGraphNode *input0, lAnimGraphNode *input1, float blendStartTime, float blendTimePeriod );

  // lAnimNode methods
  virtual void                     SetTime(float time);

  // lAnimGraphNodeTimedBlend methods
  virtual bool                     IsBlendFinished() const;

protected:
  float m_blendStartTime;
  float m_blendRate;
  float m_t;
};

// stream node
class LSCENE_API lAnimGraphNodeStream : public lAnimGraphNodeTimedBlend
{
protected:
  lAnimGraphNodeStream( lAnimGraphNode *source = NULL );

public:
  virtual ~lAnimGraphNodeStream();

  // factory methods
  static lAnimGraphNodeStream *Create( lAnimGraphNode *source = NULL );

  // lAnimGraphNodeStream methods
  lAnimGraphNode                  *GetCurrentNode();
  lAnimGraphNode                  *GetDestinationNode();

  //bSynchronize  :  if true, then the destination animation is synchronized to the current time of the current animation
  void                             BlendTo( lAnimGraphNode *destAnim, float period, bool bSynchronizeTime = false, bool bDestFromStart = false );
  void                             SetTo( lAnimGraphNode *anim );
  bool                             IsBlending();
};

END_L_NAMESPACE

#endif
