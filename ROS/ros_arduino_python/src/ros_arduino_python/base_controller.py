#!/usr/bin/env python

"""
    A base controller class for the Arduino microcontroller
    
    Borrowed heavily from Mike Feguson's ArbotiX base_controller.py code.
    
    Created for the Pi Robot Project: http://www.pirobot.org
    Copyright (c) 2010 Patrick Goebel.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details at:
    
    http://www.gnu.org/licenses
"""
import roslib; roslib.load_manifest('ros_arduino_python')
import rospy
import os

from math import sin, cos, pi
from geometry_msgs.msg import Quaternion, Twist, Pose
from nav_msgs.msg import Odometry
from tf.broadcaster import TransformBroadcaster

 
""" Class to receive Twist commands and publish Odometry data """
class BaseController:
    def __init__(self, arduino, base_frame, name="base_controllers"):
        self.arduino = arduino
        self.name = name
        self.base_frame = base_frame
        self.rate = float(rospy.get_param("~base_controller_rate", 10))
        self.timeout = rospy.get_param("~base_controller_timeout", 1.0)
        self.stopped = False
        self.debugPID = True

        pid_params = dict()
        pid_params['wheel_diameter'] = rospy.get_param("~wheel_diameter", "") 
        pid_params['wheel_track'] = rospy.get_param("~wheel_track", "")
        pid_params['encoder_resolution'] = rospy.get_param("~encoder_resolution", "") 
        pid_params['gear_reduction'] = rospy.get_param("~gear_reduction", 1.0)
        
        pid_params['AKp'] = rospy.get_param("~AKp", 20)
        pid_params['AKd'] = rospy.get_param("~AKd", 12)
        pid_params['AKi'] = rospy.get_param("~AKi", 0)
        pid_params['AKo'] = rospy.get_param("~AKo", 50)

        pid_params['BKp'] = rospy.get_param("~BKp", 20)
        pid_params['BKd'] = rospy.get_param("~BKd", 12)
        pid_params['BKi'] = rospy.get_param("~BKi", 0)
        pid_params['BKo'] = rospy.get_param("~BKo", 50)

        pid_params['CKp'] = rospy.get_param("~CKp", 20)
        pid_params['CKd'] = rospy.get_param("~CKd", 12)
        pid_params['CKi'] = rospy.get_param("~CKi", 0)
        pid_params['CKo'] = rospy.get_param("~CKo", 50)

        pid_params['DKp'] = rospy.get_param("~DKp", 20)
        pid_params['DKd'] = rospy.get_param("~DKd", 12)
        pid_params['DKi'] = rospy.get_param("~DKi", 0)
        pid_params['DKo'] = rospy.get_param("~DKo", 50)
        
        self.accel_limit = rospy.get_param('~accel_limit', 0.1)
        self.motors_reversed = rospy.get_param("~motors_reversed", False)
        self.linear_scale_correction = rospy.get_param("~linear_scale_correction",0.998)
        self.angular_scale_correction = rospy.get_param("~angular_scale_correction",0.9872)
        # Set up PID parameters and check for missing values
        self.setup_pid(pid_params)
            
        # How many encoder ticks are there per meter?
        self.ticks_per_meter = self.encoder_resolution * self.gear_reduction * 4 / (self.wheel_diameter * pi)
        self.ticks_per_meter = self.ticks_per_meter / self.linear_scale_correction

        # What is the maximum acceleration we will tolerate when changing wheel speeds?
        self.max_accel = self.accel_limit * self.ticks_per_meter / self.rate
                
        # Track how often we get a bad encoder count (if any)
        self.bad_encoder_count = 0
                        
        now = rospy.Time.now()    
        self.then = now # time for determining dx/dy
        self.t_delta = rospy.Duration(1.0 / self.rate)
        self.t_next = now + self.t_delta

        # Internal data
        self.enc_A = None            # encoder readings
        self.enc_B = None
        self.enc_C = None
        self.enc_D = None

        self.x = 0                      # position in xy plane
        self.y = 0
        self.th = 0                     # rotation in radians
        
        self.v_A = 0
        self.v_B = 0
        self.v_C = 0
        self.v_D = 0

        self.v_des_AWheel = 0             # cmd_vel setpoint
        self.v_des_BWheel = 0
        self.v_des_CWheel = 0
        self.v_des_DWheel = 0
        self.last_cmd_vel = now

        # Subscriptions
        rospy.Subscriber("cmd_vel", Twist, self.cmdVelCallback)
        
        # Clear any old odometry info
        self.arduino.reset_encoders()
        
        # Set up the odometry broadcaster
        self.odomPub = rospy.Publisher('odom', Odometry, queue_size=5)
        self.odomBroadcaster = TransformBroadcaster()
        
        rospy.loginfo("Started base controller for a base of " + str(self.wheel_track) + "m wide with " + str(self.encoder_resolution) + " ticks per rev")
        rospy.loginfo("Publishing odometry data at: " + str(self.rate) + " Hz using " + str(self.base_frame) + " as base frame")
        
    def setup_pid(self, pid_params):
        # Check to see if any PID parameters are missing
        missing_params = False
        for param in pid_params:
            if pid_params[param] == "":
                print("*** PID Parameter " + param + " is missing. ***")
                missing_params = True
        
        if missing_params:
            os._exit(1)
                
        self.wheel_diameter = pid_params['wheel_diameter']
        self.wheel_track = pid_params['wheel_track']
        self.wheel_track = self.wheel_track/self.angular_scale_correction
        self.encoder_resolution = pid_params['encoder_resolution']
        self.gear_reduction = pid_params['gear_reduction']
        
        self.AKp = pid_params['AKp']
        self.AKd = pid_params['AKd']
        self.AKi = pid_params['AKi']
        self.AKo = pid_params['AKo']

        self.BKp = pid_params['BKp']
        self.BKd = pid_params['BKd']
        self.BKi = pid_params['BKi']
        self.BKo = pid_params['BKo']

        self.CKp = pid_params['CKp']
        self.CKd = pid_params['CKd']
        self.CKi = pid_params['CKi']
        self.CKo = pid_params['CKo']

        self.DKp = pid_params['DKp']
        self.DKd = pid_params['DKd']
        self.DKi = pid_params['DKi']
        self.DKo = pid_params['DKo']
        
        self.arduino.update_pid(self.AKp, self.AKd, self.AKi, self.AKo,
                                self.BKp, self.BKd, self.BKi, self.BKo,
                                self.CKp, self.CKd, self.CKi, self.CKo,
                                self.DKp, self.DKd, self.DKi, self.DKo)

    def poll(self):
        now = rospy.Time.now()
        if now > self.t_next:                
            try:
                awheel_enc,bwheel_enc,cwheel_enc,dwheel_enc = self.arduino.get_encoder_counts()
            except:
                self.bad_encoder_count += 1
                rospy.logerr("Encoder exception count: " + str(self.bad_encoder_count))
                return
                            
            dt = now - self.then
            self.then = now
            dt = dt.to_sec()
            
            # Calculate odometry
            if self.enc_A == None and self.enc_B == None and self.enc_C == None and self.enc_D == None:
                d_A = 0
                d_B = 0
                d_C = 0
                d_D = 0
            else:
                d_A = (awheel_enc - self.enc_A) / self.ticks_per_meter
                d_B = (bwheel_enc - self.enc_B) / self.ticks_per_meter
                d_C = (cwheel_enc - self.enc_C) / self.ticks_per_meter
                d_D = (dwheel_enc - self.enc_D) / self.ticks_per_meter
            self.enc_A = awheel_enc
            self.enc_B = bwheel_enc
            self.enc_C = cwheel_enc
            self.enc_D = dwheel_enc

            va = d_A/dt
            vb = d_B/dt
            vc = d_C/dt
            vd = d_D/dt

            # va = w(R-L) = vc = v - wL
            # vb = w(R+L) = vd = v + wL
            # L  = wheel_track / 2.0
            vx = (va + vb + vc + vd) / 4.0
            vth = (vb - va + vd - vc) / (2.0 * self.wheel_track)

            dx = cos(vth*dt) * vx * dt
            dy = -sin(vth*dt) * vx * dt
            self.x += cos(self.th) * dx - sin(self.th) * dy
            self.y += sin(self.th) * dx + sin(self.th) * dy
            self.th += vth * dt

            quaternion = Quaternion()
            quaternion.x = 0.0 
            quaternion.y = 0.0
            quaternion.z = sin(self.th / 2.0)
            quaternion.w = cos(self.th / 2.0)
    
            # Create the odometry transform frame broadcaster.
            self.odomBroadcaster.sendTransform(
                (self.x, self.y, 0), 
                (quaternion.x, quaternion.y, quaternion.z, quaternion.w),
                rospy.Time.now(),
                self.base_frame,
                "odom"
                )
    
            odom = Odometry()
            odom.header.frame_id = "odom"
            odom.child_frame_id = self.base_frame
            odom.header.stamp = now
            odom.pose.pose.position.x = self.x
            odom.pose.pose.position.y = self.y
            odom.pose.pose.position.z = 0
            odom.pose.pose.orientation = quaternion
            odom.twist.twist.linear.x = vx
            odom.twist.twist.linear.y = 0
            odom.twist.twist.angular.z = vth

            self.odomPub.publish(odom)
            
            if now > (self.last_cmd_vel + rospy.Duration(self.timeout)):
                self.v_des_AWheel = 0
                self.v_des_BWheel = 0
                self.v_des_CWheel = 0
                self.v_des_DWheel = 0
            if self.v_A < self.v_des_AWheel:
                self.v_A += self.max_accel
                if self.v_A > self.v_des_AWheel:
                    self.v_A = self.v_des_AWheel
            else:
                self.v_A -= self.max_accel
                if self.v_A < self.v_des_AWheel:
                    self.v_A = self.v_des_AWheel
            
            if self.v_B < self.v_des_BWheel:
                self.v_B += self.max_accel
                if self.v_B > self.v_des_BWheel:
                    self.v_B = self.v_des_BWheel
            else:
                self.v_B -= self.max_accel
                if self.v_B < self.v_des_BWheel:
                    self.v_B = self.v_des_BWheel

            if self.v_C < self.v_des_CWheel:
                self.v_C += self.max_accel
                if self.v_C > self.v_des_CWheel:
                    self.v_C = self.v_des_CWheel
            else:
                self.v_C -= self.max_accel
                if self.v_C < self.v_des_CWheel:
                    self.v_C = self.v_des_CWheel

            if self.v_D < self.v_des_DWheel:
                self.v_D += self.max_accel
                if self.v_D > self.v_des_DWheel:
                    self.v_D = self.v_des_DWheel
            else:
                self.v_D -= self.max_accel
                if self.v_D < self.v_des_DWheel:
                    self.v_D = self.v_des_DWheel
            # Set motor speeds in encoder ticks per PID loop
            if not self.stopped:
                self.arduino.drive(self.v_A, self.v_B, self.v_C,self.v_D)
                # if self.debugPID:
                #     self.AVelPub.publish(self.v_A)
                #     self.BVelPub.publish(self.v_B)
                #     self.CVelPub.publish(self.v_C)
                #     self.DVelPub.publish(self.v_D)
                
            self.t_next = now + self.t_delta
            
    def stop(self):
        self.stopped = True
        self.arduino.drive(0, 0, 0, 0)
            
    def cmdVelCallback(self, req):
        # Handle velocity-based movement requests
        self.last_cmd_vel = rospy.Time.now()
        
        x = req.linear.x         # m/s
        th = req.angular.z       # rad/s

        vA = x - th * self.wheel_track / 2.0
        vB = x + th * self.wheel_track / 2.0
        vC = vA
        vD = vB
        self.v_des_AWheel = int(vA * self.ticks_per_meter / self.arduino.PID_RATE)
        self.v_des_BWheel = int(vB * self.ticks_per_meter / self.arduino.PID_RATE)
        self.v_des_CWheel = int(vC * self.ticks_per_meter / self.arduino.PID_RATE)
        self.v_des_DWheel = int(vD * self.ticks_per_meter / self.arduino.PID_RATE)
        self.arduino.drive_m_per_s(vA,vB,vC,vD)
        rospy.loginfo("A wheel:" + str(self.v_des_AWheel) + ", B wheel:" +
                      str(self.v_des_BWheel) + ", C wheel:" + str(self.v_des_CWheel) + ", D wheel:" + str(self.v_des_DWheel))
