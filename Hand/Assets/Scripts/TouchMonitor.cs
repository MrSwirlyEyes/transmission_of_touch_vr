using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

public class TouchMonitor : MonoBehaviour {

	short pressure;

	// Use this for initialization
	void Start () {
	}
	
	// Update is called once per frame
	void Update () {
		
	}

	void OnCollisionEnter(Collision collision) {
		Debug.Log ("Entered " + collision.contacts[0].thisCollider.name);
		pressure = (short) (collision.impulse / Time.deltaTime).magnitude;
	}

	void OnCollisionExit(Collision collision) {
		setVibes ("", 0);
	}

	void OnCollisionStay(Collision collision) {
		Debug.Log ("Stay " + collision.contacts[0].thisCollider.name);

		if (collision.impulse.magnitude != 0) {
			pressure = (short)(collision.impulse / Time.deltaTime).magnitude;
		}

		foreach (ContactPoint contact in collision.contacts) {
			Debug.Log (contact.thisCollider.name + " hit " + contact.otherCollider.name);
		}

		short setVibe = map(pressure, 0, 10000, 0, 255);
		Debug.Log ("setVibe: " + setVibe);

		setVibes (collision.contacts [0].thisCollider.name, setVibe);
	}

	void setVibes(string name, short value) {
		switch (name) {
		case "RightHandThumb1":
		case "RightHandThumb2":
		case "RightHandThumb3":
		case "RightHandThumb4":
			Communicator.instance.vibes.thumb = value;
			break;
		case "RightHandIndex1":
		case "RightHandIndex2":
		case "RightHandIndex3":
		case "RightHandIndex4":
			Communicator.instance.vibes.index = value;
			break;
		case "RightHandMiddle1":
		case "RightHandMiddle2":
		case "RightHandMiddle3":
		case "RightHandMiddle4":
			Communicator.instance.vibes.middle = value;
			break;
		case "RightHandRing1":
		case "RightHandRing2":
		case "RightHandRing3":
		case "RightHandRing4":
			Communicator.instance.vibes.ring = value;
			break;
		case "RightHandPinky1":
		case "RightHandPinky2":
		case "RightHandPinky3":
		case "RightHandPinky4":
			Communicator.instance.vibes.pinky = value;
			break;
		default:
			Communicator.instance.vibes.thumb = value;
			Communicator.instance.vibes.index = value;
			Communicator.instance.vibes.middle = value;
			Communicator.instance.vibes.ring = value;
			Communicator.instance.vibes.pinky = value;
			break;
		}
	}

	short map(short x, int in_min, int in_max, int out_min, int out_max) {
		Debug.Log ("pressure: " + x);
		float slope = (float)(out_max - out_min)/(in_max-in_min);
		float stretched = slope * (x-in_min);
//		float b = ((float)out_max / out_min / slope);
		Debug.Log ("slope: " + slope);
		Debug.Log ("stretched: " + stretched);
//		Debug.Log ("b: " + b);
		short returnable = (short)(stretched);
		if (returnable > 255)
			return 255;
		if (returnable < 0)
			return 0;
		else
			return returnable;
	}
}
