using UnityEngine;
using System;
using System.Collections;
using System.Reflection;
//using System.IO.Ports;
using System.IO;

public class Communicator : MonoBehaviour {

	public static Communicator instance = null;
	public string port;
	public int baudrate;

	private SerialPort stream;

	public int minFlex = 20;
	public int maxFlex = 110;
	public int minFinger = 10;
	public int maxFinger = 90;
	public int maxThumb = 50;

	/* DATA FOR THE HAND */
	public struct KnuckleValues {
		public int index, middle, ring, pinky, thumb;
	}

	public KnuckleValues knuckles;
	/* END HAND DATA */

	public void Open() {
		stream = new SerialPort (port, baudrate);
		stream.ReadTimeout = 50;
		stream.Open ();
	}

	public void WriteToArduino(byte[] message) {
		stream.Write(message,0,message.Length);
		stream.BaseStream.Flush ();
	}

	public void WriteToArduino(string message) {
		stream.WriteLine(message);
		//		stream.BaseStream.Flush ();
	}

	public string ReadFromArduino (int timeout = 100) {
		stream.ReadTimeout = timeout;
		try {
			return stream.ReadLine ();
		} catch (TimeoutException e) {
			Debug.Log ("Timeout Exception " + e);
			stream.BaseStream.Flush ();
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
			//			try {
			dataString = stream.ReadLine ();
			Debug.Log("Receiving " + dataString);
			//				stream.BaseStream.Flush();
			//			} catch (TimeoutException e) {
			//				Debug.LogError(e);
			//				dataString = null;
			//			}

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
		if (instance == null) {
			instance = this;
		} else if (instance != this) {
			Destroy (gameObject);
		}
		Open ();
	}

	void Update() {
		WriteToArduino ("PING");
//		StartCoroutine (
//			AsynchronousReadFromArduino (handleData, writeDefault
//				//				() => Debug.LogError ("READ FAIL"),
//				//				10f
//			)
//		);
	}

	void handleData(string s) {
		char[] delimiters = { ',' };
		string[] values = s.Split (delimiters);

		knuckles.thumb = map(System.Convert.ToInt32 (values [0]),maxFlex,minFlex,minFinger,maxThumb);
		knuckles.index = map(System.Convert.ToInt32 (values [1]),maxFlex,minFlex,minFinger,maxFinger);
		knuckles.middle =  map(System.Convert.ToInt32 (values [2]),maxFlex,minFlex,minFinger,maxFinger);
		knuckles.ring =  map(System.Convert.ToInt32 (values [3]),maxFlex,minFlex,minFinger,maxFinger);
		knuckles.pinky = map(System.Convert.ToInt32 (values [4]),maxFlex,minFlex,minFinger,maxFinger);

	}

	int map(int x, int in_min, int in_max, int out_min, int out_max) {
		return (x-in_min)*(out_max-out_min)/(in_max-in_min) + out_min;
	}

	void writeDefault() {
		knuckles.thumb = 40;
		knuckles.index = 40;
		knuckles.middle = 40;
		knuckles.ring = 40;
		knuckles.pinky = 40;
	}
}
