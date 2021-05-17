# ERIAN
This Github workspace covers Shiyu's documentation and materials of ERIAN

## Learning Doc Folder
- Autoware.pdf 
- Commands_in_Linux.pdf
- Image_Labeling.pdf
- Rosbag_with_ImageLabel.pdf
- Rosbridge.pdf
- Rosdep_Debugging.pdf
- RosMaster__Slave.pdf
- Ros_in_Windows.pdf
- ROS_Overall.pdf

## OSE3AD Folder
There are three designs for FMU: <br/>
FMU design 1 : Install autoware in Linux and connect Window ROS with it through Master & Slave <br/>
FMU design 2 : Build Buggy code in Windows <br/>
FMU design 3 : Implement bus algorithm in Matlab Simulink Model then generate FMU using Fmi.LAB (AVL) <br/>

### FMU_Design Folder
- FMU_design1&2.pdf
- FMU_design3.pdf
- FMU_report1.pdf
- FMU_report2.pdf    
- FMU_report3.pdf

### Learning_Material
- AVL_ModelCONNECT.pdf      (Personal learning Notes for ModelConncet based on Alex's video)
- FMI_for_ModelExchange_and_CoSimulation_v2.0.pdf       (Reading material for FMU 2.O)
- CoordinationTransformation.fmu  (Sample FMU for co-simulation)   *Can use FMPy GUI to check FMU's configuration 
- add_client.fmu (Sample FMU for add_two_ints example)
- Rectifier.fmu  (Sample FMU of online example)

### AVL_Training Folder
- Traning_Notes.pdf         (Traning notes for two-days training)
- Traning_Steps.pptx        (Traning step-by-step details)

### Project_Materials Folder
- modelConnect_matrix.xlsx  (Matrix relations of VTD,VSM and FMU)
- moduleManager.xml         (Sensor example stored in VTD)
- NTU_2 (waypoint).xml      (Scenarios example stored in VTD)

### ModelConnect_Projects Folder <Must build it in ROS workspace before run this rosservice>
- beginner_tutorial     (add_two_ints example)
- mapping               (mapping function)
