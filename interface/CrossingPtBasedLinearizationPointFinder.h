#ifndef CrossingPtBasedLinearizationPointFinder_H
#define CrossingPtBasedLinearizationPointFinder_H

#include "RecoVertex/VertexTools/interface/LinearizationPointFinder.h"
#include "RecoVertex/VertexPrimitives/interface/DummyRecTrack.h"
#include "RecoVertex/VertexTools/interface/ModeFinder3d.h"
#include "RecoVertex/VertexTools/interface/RecTracksDistanceMatrix.h"

  /** A linearization point finder. It works the following way:
   *  1. Calculate in an optimal way 'n_pairs' different crossing points.
   *     Optimal in this context means the following:
   *     a. Try to use as many different tracks as possible;
   *        avoid using the same track all the time.
   *     b. Use the most energetic tracks.
   *     c. Try not to group the most energetic tracks together.
   *        Try to group more energetic tracks with less energetic tracks.
   *        We assume collimated bundles here, so this is why.
   *     d. Perform optimally. Do not sort more tracks (by total energy, see b)
   *        than necessary.
   *     e. If n_pairs >= (number of all possible combinations),
   *        do not leave any combinations out.
   *     ( a. and e. are almost but not entirely fulfilled in the current impl )
   *  2. Apply theAlgo on the n points.
   */

class CrossingPtBasedLinearizationPointFinder : public LinearizationPointFinder
{
public:
  /** \param n_pairs: how many track pairs will be considered (maximum)
   *                  a value of -1 means full combinatorics.
   */
  CrossingPtBasedLinearizationPointFinder( const ModeFinder3d & algo,
     const signed int n_pairs = 5 );

  /** This constructor exploits the information stored in a 
   *  RecTracksDistanceMatrix object.
   *  \param n_pairs: how many track pairs will be considered (maximum)
   *                  a value of -1 means full combinatorics.
   */

  CrossingPtBasedLinearizationPointFinder( 
      const RecTracksDistanceMatrix * m, const ModeFinder3d & algo,
      const signed int n_pairs = -1 );

  CrossingPtBasedLinearizationPointFinder(
      const CrossingPtBasedLinearizationPointFinder & );

  ~CrossingPtBasedLinearizationPointFinder();

/** Method giving back the Initial Linearization Point.
 */
  virtual GlobalPoint getLinearizationPoint(const vector<DummyRecTrack> & ) const;
  virtual GlobalPoint getLinearizationPoint(const vector<FreeTrajectoryState> & ) const;

  virtual CrossingPtBasedLinearizationPointFinder * clone() const {
    return new CrossingPtBasedLinearizationPointFinder ( * this );
  };
protected:
  const bool useMatrix;
  signed int theNPairs;
  const RecTracksDistanceMatrix *theMatrix;

private:
  /// calls (*theAglo) (input)
  /// can optionally save input / output in .root file
  GlobalPoint find ( const vector<pair <GlobalPoint , float> > & ) const;
private:
  ModeFinder3d * theAlgo;

  /** Private struct to order tracks by momentum
   */
  struct CompareTwoDummyRecTracks {
    int operator() ( const DummyRecTrack & a, const DummyRecTrack & b ) {
       return a.impactPointState().globalMomentum().mag() >
              b.impactPointState().globalMomentum().mag();
    };
  };
  vector <DummyRecTrack> getBestTracks ( const vector<DummyRecTrack> & ) const;
  GlobalPoint useFullMatrix ( const vector<DummyRecTrack> & ) const;
  GlobalPoint useAllTracks  ( const vector<DummyRecTrack> & ) const;
};

#endif
