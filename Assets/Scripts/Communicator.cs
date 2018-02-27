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

	public int minFlex;
	public int maxFlex;
	public int minFinger;
	public int maxFinger;
	public int maxThumb;

	private int numVibes = 5;

	/* DATA FOR THE HAND */
	public struct KnuckleValues {
		public int index, middle, ring, pinky, thumb;
	}

	public struct VibeValues {
		public short index, middle, ring, pinky, thumb;
	}

	public KnuckleValues knuckles;
	public VibeValues vibes;
	/* END HAND DATA */

	private bool reading = false;

	



	public void Open() {
		stream = new SerialPort (port, baudrate);
//		Debug.Log ("initialized stream on " + port + " at " + baudrate);
		stream.ReadTimeout = 10;
//		stream.DataReceived += new SerialDataReceivedEventHandler(handleData);
		stream.Open ();
	}

	public void WriteToArduino() {
		short[] vibeValues = new short[] { vibes.thumb, vibes.index, vibes.middle, vibes.ring, vibes.pinky };
		byte[] bytes = new byte[numVibes * sizeof(short)];
		Buffer.BlockCopy (vibeValues, 0, bytes, 0, bytes.Length);
		stream.Write(bytes,0,bytes.Length);
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

		vibes.thumb = 0;
		vibes.index = 0;
		vibes.middle = 0;
		vibes.ring = 0;
		vibes.pinky = 0;
	}

	void Update() {

		if (!reading) {
			WriteToArduino ();
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

		knuckles.thumb = mapInvert(System.Convert.ToInt32 (values [0]),minFlex,maxFlex,minFinger,maxThumb);
		knuckles.index = mapInvert(System.Convert.ToInt32 (values [1]),minFlex,maxFlex,minFinger,maxFinger);
		knuckles.middle =  mapInvert(System.Convert.ToInt32 (values [2]),minFlex,maxFlex,minFinger,maxFinger);
		knuckles.ring =  mapInvert(System.Convert.ToInt32 (values [3]),minFlex,maxFlex,minFinger,maxFinger);
		knuckles.pinky = mapInvert(System.Convert.ToInt32 (values [4]),minFlex,maxFlex,minFinger,maxFinger);
//		Debug.Log ("Index: " + knuckles.index);
	}

	int mapInvert(int x, int in_min, int in_max, int out_min, int out_max) {
		float slope = (float)(out_min - out_max)/(in_max-in_min);
		float stretched = slope * (x-in_min);
		float b = ((float)out_max / in_min / slope);
		return out_max + (int)(stretched+b);
	}

	void writeDefault() {
		knuckles.thumb = 40;
		knuckles.index = 40;
		knuckles.middle = 40;
		knuckles.ring = 40;
		knuckles.pinky = 40;
	}
}
