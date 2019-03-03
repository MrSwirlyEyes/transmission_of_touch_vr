using UnityEngine;
using System;
using System.Collections;
using System.Reflection;
using System.IO.Ports;
using UnityEngine.UI;

public struct SensorPacket {
	public int s1, s2, s3, s4, s5;
}

public class ArduinoConnector : MonoBehaviour {

	public string port = "COM6";
	public int baudrate = 9600;
	public float intensityConversion = (float) 1.5 / 395;
	private float intensityB = 1.942f;

	public Slider slider1, slider2, slider3, slider4, slider5;
	private SerialPort stream;

	private SensorPacket pkt;

	public static int numChildren = 5;
	private Light[] children = new Light[numChildren];

	public void Open() {
		stream = new SerialPort (port, baudrate);
		stream.ReadTimeout = 50;
		stream.Open ();
	}

	public void WriteToArduino(string message) {
		stream.WriteLine (message);
		stream.BaseStream.Flush ();
	}

	public string ReadFromArduino (int timeout = 0) {
		stream.ReadTimeout = timeout;
		try {
			return stream.ReadLine ();
		} catch (TimeoutException e) {
			Debug.Log ("Timeout Exception " + e);
			return null;
		}
	}

	public IEnumerator AsynchronousReadFromArduino (Action<string> callback, 
													Action fail = null,
													float timeout = float.PositiveInfinity
													) {
		DateTime initialTime = DateTime.Now;
		DateTime nowTime;
		TimeSpan diff = default(TimeSpan);

		 string dataString = null;

		do {
			try {
				dataString = stream.ReadLine ();
			} catch (TimeoutException) {
				dataString = null;
			}

			if (dataString != null) {
				callback (dataString);
				yield break;
			} else {
				yield return new WaitForSeconds (0.05f);
			}

			nowTime = DateTime.Now;
			diff = nowTime - initialTime;
		} while (diff.Milliseconds < timeout);

		if (fail != null)
			fail ();
		yield return null;
	}

	public void Close() {
		stream.Close ();
	}

	// Use this for initialization
	void Start () {
		Open ();
		for (int i = 0; i < numChildren; i++) {
			children [i] = transform.GetChild (i).GetComponent<Light> ();
		}
	}
	
	// Update is called once per frame
	void Update () {
		WriteToArduino ("PING");
		StartCoroutine (
			AsynchronousReadFromArduino (handleData,
				() => Debug.LogError ("Error!"),
				10f
			)
		);
		WriteToArduino ("ECHO " + slider1.value + "," +
			slider2.value + "," +
			slider3.value + "," +
			slider4.value + "," +
			slider5.value
		);

//		WriteToArduino ("ECHO " + slider1.value);
	}

	void OnMouseDown() {

	}

	private void handleData(string s) {
		char[] delimiters = { ',' };
		string[] values = s.Split (delimiters);

		pkt.s1 = System.Convert.ToInt32 (values [0]);
		pkt.s2 = System.Convert.ToInt32 (values [1]);
		pkt.s3 = System.Convert.ToInt32 (values [2]);
		pkt.s4 = System.Convert.ToInt32 (values [3]);
		pkt.s5 = System.Convert.ToInt32 (values [4]);

		for (int i = 0; i < numChildren; i++) {
			(children [i]).intensity = (float) ((System.Convert.ToInt32 (values [i])*intensityConversion) - intensityB);
		}
	}
}
