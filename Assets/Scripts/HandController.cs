using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HandController : MonoBehaviour {

	public Transform index1;
	public Transform index2;
	public Transform index3;

	public Transform middle1;
	public Transform middle2;
	public Transform middle3;

	public Transform ring1;
	public Transform ring2;
	public Transform ring3;

	public Transform pinky1;
	public Transform pinky2;
	public Transform pinky3;

	public Transform thumb1;
	public Transform thumb2;
	public Transform thumb3;

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		index2.localEulerAngles = new Vector3(0, 0, Communicator.instance.knuckles.index);
		index3.localEulerAngles = new Vector3(0, 0, Communicator.instance.knuckles.index);

		middle2.localEulerAngles = new Vector3(0, 0, Communicator.instance.knuckles.middle);
		middle3.localEulerAngles = new Vector3(0, 0, Communicator.instance.knuckles.middle);

		ring2.localEulerAngles = new Vector3(0, 0, Communicator.instance.knuckles.ring);
		ring3.localEulerAngles = new Vector3(0, 0, Communicator.instance.knuckles.ring);

		pinky2.localEulerAngles = new Vector3(0, 0, Communicator.instance.knuckles.pinky);
		pinky3.localEulerAngles = new Vector3(0, 0, Communicator.instance.knuckles.pinky);

		thumb2.localEulerAngles = new Vector3(0, 0, Communicator.instance.knuckles.thumb);
		thumb3.localEulerAngles = new Vector3(0, 0, Communicator.instance.knuckles.thumb);

	}
}
