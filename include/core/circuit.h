// **************************************************************************
// File       [ circuit.h ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2010/12/29 created ]
// **************************************************************************

#ifndef _CORE_CIRCUIT_H_
#define _CORE_CIRCUIT_H_

#include <set>

#include "interface/netlist.h"

#include "gate.h"

//1/25 update
#include <queue>

//
namespace CoreNs {

//=========================================================================// 
//11/30 update
class cycle{
    public:
        cycle( vector<int>& v , const int& nframe);
        
        void getPI();
        void getPO();
        void remove_redundant();
        void setup(Gate* cirgates_);
        bool single_output(int target , Gate* cirgates_);
        void setSource(int src){source = src;}
        vector<int> nodes;
        int npi_;
        int npo_;
        int ngate_;
        int nframe_;
        Gate* gates_;
        int getSrc(){return source;}
        int getTar(){return target_;}
        
        // for debug 
        void print();
        void printPI();
        void printPO();
        bool checkSingleOutput(Gate* cirgates_);
        bool checkTargetfanin(Gate* cirgates_){
            if(cirgates_[target_].nfi_ > 2){
                return true;
            }else{
                return false;
            }
        }

    private:
        int target_;
        int source;
        vector< vector<int> > pi; // supergate primary input ex : vector<int>[0] = 2,3 (gate 2 , fanin 3)
        vector< vector<int> > po; // supergate primary output

};

inline cycle::cycle( vector<int>& v , const int& nframe){
    nodes.assign(v.begin(), v.end());
    nframe_ = nframe;   
}

//=========================================================================// 


class Circuit {
private:
    //=========================================================================// 
    //11/30 update
    vector< cycle > cycles_candidate;
    vector< cycle > cycles;
    vector< int > fanout_branches;
    
    //=========================================================================//
    
public:
    Circuit();
    Circuit(cycle* cyc_);
    ~Circuit();

	// specify how to connect multiple time frames of circuits
	// CAPTURE means Launch-on-capture;  SHIFT means launch-on-shift
    enum            tfConnectType { CAPTURE = 0, SHIFT };

    // build circuit from netlist
    bool            build(IntfNs::Netlist * const nl, const int &nframe = 1,
                          const tfConnectType &type = CAPTURE);

    // info for one time frame
    IntfNs::Netlist *nl_;         // corresponding netlist
    int             npi_;         // number of PIs
    int             nppi_;        // number of PPIs
    int             npo_;         // number of POs
    int             ncomb_;       // number of combinational gates
    int             ngate_;       // number of gates
    int             nnet_;        // number of nets
    int             lvl_;         // circuit level, starting from inputs

    // info for multiple time frames
    int             nframe_;      // number of time frame
    tfConnectType        connType_;    // time frame connection type
    int             tgate_;       // number of total gates
    int             tlvl_;        // total level

    // structure
    // **********************************************************************
    // frame 1                frame 2                      frame n
    // |--- ---- ---- --- ----|--- ---- ---- --- ----|     |--- ---- ----
    // |PI1|PPI1|cell|PO1|PPO1|PI2|PPI2|cell|PO2|PPO2| ... |PIn|PPIn|cell|...
    // |--- ---- ---- --- ----|--- ---- ---- --- ----|     |--- ---- ----
    // **********************************************************************
    Gate            *gates_;      // gate array
    int             *fis_;        // fanin net array
    int             *fos_;        // fanout net array
    int             *cellToGate_; // map cells in netlist to gates
    int             *portToGate_; // map ports in netlist to gates
    //4/12 update
    vector< int > faultorder;
protected:
    // for circuit building
    void createMap();
    void calNgate();
    void calNnet();
    void createGate();
    void createPi(int &nfo);
    void createPpi(int &nfo);
    void createComb(int &nfi, int &nfo);
    void createPmt(const int &id, const IntfNs::Cell * const c,
                   const IntfNs::Pmt * const pmt, int &nfi, int &nfo);
    void detGateType(const int &id, const IntfNs::Cell * const c,
                     const IntfNs::Pmt * const pmt);
    void createPo(int &nfi);
    void createPpo(int &nfi);
    void connectFrame();
    void assignFiMinLvl();

    void runScoap();
    //=========================================================================//
    //1/25 update : find_fanout_branches() , bfs(int idx)
    //2/1  update : find_reconvergence(int idx) , reset() , pop_fanout_branches(int u)
    void find_fanout_branches();
    void bfs(int idx);
    void find_reconvergence(int idx);
    void reset();
    void pop_fanout_branches(int u);
    bool cycle_available(cycle& c);
    //=========================================================================//
};

inline Circuit::Circuit() {
    nl_         = NULL;
    npi_        = 0;
    nppi_       = 0;
    npo_        = 0;
    ncomb_      = 0;
    ngate_      = 0;
    nnet_       = 0;
    lvl_        = -1;
    nframe_     = 1;
    connType_   = CAPTURE;
    gates_      = NULL;
    cellToGate_ = NULL;
    portToGate_ = NULL;
}

inline Circuit::Circuit(cycle* cyc_) {
    nl_         = NULL;
    npi_        = cyc_->npi_;
    nppi_       = 0;
    npo_        = cyc_->npo_;
    ncomb_      = 0;
    ngate_      = cyc_->ngate_;
    nnet_       = 0;
    lvl_        = -1;
    nframe_     = 1;
    connType_   = CAPTURE;
    gates_      = cyc_->gates_;
    cellToGate_ = NULL;
    portToGate_ = NULL;


}

inline Circuit::~Circuit() {
    delete [] gates_;
    delete [] fis_;
    delete [] fos_;
    delete [] cellToGate_;
    delete [] portToGate_;
}





};

#endif

