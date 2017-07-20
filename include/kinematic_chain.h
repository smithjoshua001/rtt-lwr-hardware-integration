#ifndef _KINEMATIC_CHAIN_H_
#define _KINEMATIC_CHAIN_H_

// RST-RT includes
#include <rst-rt/kinematics/JointAngles.hpp>
#include <rst-rt/kinematics/JointVelocities.hpp>
#include <rst-rt/dynamics/JointTorques.hpp>
#include <rst-rt/dynamics/JointImpedance.hpp>
#include <rst-rt/robot/JointState.hpp>

#include <friremote_rt.h>

#include <control_modes.h>
#include <fricomm_rt.h>

using namespace rstrt::kinematics;
using namespace rstrt::dynamics;
using namespace rstrt::robot;


typedef cogimon::jointCtrl<JointAngles> position_ctrl;
typedef cogimon::jointCtrl<JointImpedance> impedance_ctrl;
typedef cogimon::jointCtrl<JointTorques> torque_ctrl;

typedef cogimon::jointFeedback<JointState> full_fbk;


class KinematicChain {
public:
    KinematicChain(const std::string& chain_name, const std::vector<std::string> &joint_names,
                   RTT::DataFlowInterface& , friRemote* friInst);
    ~KinematicChain(){}

    std::string getKinematicChainName();
    unsigned int getNumberOfDOFs();
    std::string getCurrentControlMode();
    std::vector<std::string> getJointNames();
    std::vector<std::string> getControllersAvailable();
    bool initKinematicChain();
    bool resetKinematicChain();
    bool setControlMode(const std::string& controlMode);
    void sense();
    void getCommand();
    void move();
void stop();
    std::string printKinematicChainInformation();
    std::vector<RTT::base::PortInterface*> getAssociatedPorts();

    bool recieved;

    boost::shared_ptr<position_ctrl> position_controller;
    boost::shared_ptr<impedance_ctrl> impedance_controller;
    boost::shared_ptr<torque_ctrl> torque_controller;

    boost::shared_ptr<full_fbk> full_feedback;

    void setGravity(bool g);
private:

    RTT::nsecs time_now, last_time;
    std::string _kinematic_chain_name;
    std::vector<std::string> _controllers_name;
    unsigned int _number_of_dofs;
    RTT::DataFlowInterface& _ports;

    std::vector<RTT::base::PortInterface*> _inner_ports;

    //gazebo::physics::ModelPtr _model;
    friRemote* _fri_inst;
    float* _last_pos, * zero_vector;
    FRI_QUALITY last_quality;
    std::string _krc_ip;
    std::string _current_control_mode;
    //gazebo::physics::JointControllerPtr _gazebo_position_joint_controller;
    std::vector<std::string> _joint_names;
    std::map<std::string, int> _map_joint_name_index;
    Eigen::VectorXf _joint_pos, _joint_trq, _joint_stiff,_joint_damp;
    RTT::OutputPort<Eigen::Matrix<float,7,7>> output_M_port;
    RTT::OutputPort<rstrt::dynamics::JointTorques> estExtTorques_port;
    rstrt::dynamics::JointTorques estExtTorques;
    Eigen::Matrix<float,7,7> output_M_var;
    bool include_gravity;

	std::vector<double> _initial_joints_configuration;

    bool setController(const std::string& controller_type);
    void setFeedBack();
    bool setJointNamesAndIndices();
    //bool initGazeboJointController();
    bool initKRC();
    std::vector<int> getJointScopedNames();
    void setInitialPosition(const bool use_model);
    void setInitialImpedance();

};



#endif
