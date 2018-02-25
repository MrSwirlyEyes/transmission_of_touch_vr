using UnityEngine;
using System;
using System.Collections;
using System.Reflection;
using System.IO.Ports;

public class Communicator : MonoBehaviour {

	public static Communicator instance = null;
	public string port = "COM6";
	public int baudrate = 9600;

	private SerialPort stream;

	public int minFlex = 60;
	public int maxFlex = 240;
	public int minFinger = 10;
	public int maxFinger = 90;
	public int maxThumb = 50;

	/* DATA FOR THE HAND */
	public struct KnuckleValues {
		public int index, middle, ring, pinky, thumb;
	}

	public KnuckleValues knuckles;
	/* END HAND DATA */

	private bool reading = false;

	



	public void Open() {
		stream = new SerialPort (port, baudrate);
//		Debug.Log ("initialized stream on " + port + " at " + baudrate);
		stream.ReadTimeout = 10;
//		stream.DataReceived += new SerialDataReceivedEventHandler(handleData);
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

	public IEnumerator ReadFromArduino (Action<string> callback) {
		string dataString = null;

		while (true) {
			try {
				dataString = stream.ReadLine ();
//				stream.BaseStream.Flush();
			} catch (TimeoutException) {
//				Debug.LogError(e);
				dataString = null;
			}

			if (dataString != null) {
				callback (dataString);
				reading = false;
				yield break;
			} else {
				yield return new WaitForSeconds (0.05f);
			}
		}
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

		if (!reading) {
			reading = true;
			StartCoroutine (
				ReadFromArduino (handleData)
			);
		}
	}

//	void handleData(object sender, SerialDataReceivedEventArgs e) {
	void handleData(string inData) {
//		SerialPort sp = (SerialPort)sender;
//		string inData = sp.ReadExisting ();

		char[] delimiters = { ',' };
		string[] values = inData.Split (delimiters);
//		Debug.Log("Receiving " + inData);

		knuckles.thumb = map(System.Convert.ToInt32 (values [0]),minFlex,maxFlex,minFinger,maxThumb);
		knuckles.index = map(System.Convert.ToInt32 (values [1]),maxFlex,minFlex,minFinger,maxFinger);
		knuckles.middle =  map(System.Convert.ToInt32 (values [2]),maxFlex,minFlex,minFinger,maxFinger);
		knuckles.ring =  map(System.Convert.ToInt32 (values [3]),maxFlex,minFlex,minFinger,maxFinger);
		knuckles.pinky = map(System.Convert.ToInt32 (values [4]),maxFlex,minFlex,minFinger,maxFinger);
		Debug.Log ("Thumb: " + knuckles.thumb);
	}

	int map(int x, int in_min, int in_max, int out_min, int out_max) {
		Debug.Log (x);
		Debug.Log (((x - in_min) * ((out_max - out_min) / (in_max - in_min))) + out_min);
		return (int) (out_max - (x - in_min)*1f*(out_max - out_min)/(in_max - in_min) + out_min);
	}

	void writeDefault() {
		knuckles.thumb = 40;
		knuckles.index = 40;
		knuckles.middle = 40;
		knuckles.ring = 40;
		knuckles.pinky = 40;
	}
}
