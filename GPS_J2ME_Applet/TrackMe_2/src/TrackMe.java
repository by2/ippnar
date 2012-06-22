import java.io.IOException;
import java.io.OutputStream;
import javax.bluetooth.BluetoothStateException;
import javax.bluetooth.DiscoveryAgent;
import javax.bluetooth.LocalDevice;
import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;
import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import javax.microedition.location.*;

public class TrackMe extends MIDlet implements CommandListener, LocationListener {

	private Display display;
	private Form form;
	private Command exit = new Command("Exit", Command.EXIT, 1);
	private Command start = new Command("Start", Command.SCREEN, 1);
	private Command stop = new Command("Stop", Command.SCREEN, 1);
	private int sec = 1;
        private StringItem state = new StringItem("STATUS: ", "\nInitializing");
        private StringItem linkState = new StringItem("LINKSTATE:", "\nInitializing");
	private StringItem info = new StringItem("LOCATION: ","\nUnknown");
        private String data = "1|1|";                                           //device not ready
	private LocationProvider locationProvider = null;
        private LocalDevice localBluetoothDevice;
        private String url = "btspp://001FE1BE6A7F:3";
        private StreamConnection connection;
        private OutputStream output;
        private boolean closeFlag = false;
        private boolean bterrFlag = false;

//constructor >> initialise variables + location provider + bluetooth
	public TrackMe(){
		display = Display.getDisplay (this);
		form = new Form("TrackMe");
		form.addCommand(exit);
		form.addCommand(start);
		form.setCommandListener(this);
                form.append(state);
                form.append(linkState);
		form.append(info);
	}

	public void commandAction(Command c, Displayable s) {
		if (c == exit) {
			exit();
		}
		if(c == start){
			form.removeCommand(start);
			//start querying GPS data :
			new Thread(){
				public void run(){
					locationProvider.setLocationListener(TrackMe.this, sec, -1, -1);
				}
			}.start();
			form.addCommand(stop);
		}
		if(c == stop){
			form.removeCommand(stop);
			// Stop querying GPS data :
			new Thread(){
				public void run(){
					locationProvider.setLocationListener(null, -1, -1, -1);
				}
			}.start();
			form.addCommand(start);
		}
	}

	public void startApp () {
            display.setCurrent(form);
            try {
                locationProvider = LocationProvider.getInstance(null);
                state.setText("\nReady");
            } catch(LocationException e1){
                state.setText("\nGPS Device Error");
            }
            try{
                localBluetoothDevice = LocalDevice.getLocalDevice();
                localBluetoothDevice.setDiscoverable(DiscoveryAgent.GIAC);
                try{
                    connection = (StreamConnection)Connector.open(url);
                    linkState.setText("\nConnected to Abhi");
                    output = connection.openOutputStream();
                    closeFlag = true;
                } catch(IOException e2){
                    //do nothing, bluetooth is neccessarily present
                    bterrFlag = true;
                    linkState.setText("\nIO Error");
                }
            } catch (BluetoothStateException e3){
                //do nothing, bluetooth is neccessarily present
                bterrFlag = true;
                linkState.setText("\nBlueTooth Error");
            }
     
        }

	public void pauseApp () {}

	public void destroyApp (boolean forced) {
            if(closeFlag == true){
                try{
                    output.close();
                    connection.close();
                } catch(IOException e4){
                //yo
                }
            }
        }

	public void exit(){
		destroyApp(false);
		notifyDestroyed();
	}

	public void locationUpdated(LocationProvider provider, Location location){
		if (location != null && location.isValid()) {
                    state.setText("\nGPS OK");
                    QualifiedCoordinates qc = location.getQualifiedCoordinates();
                    info.setText(
                            "\nLatitude:\n"
                            + qc.getLatitude()
                            + "\nLongitude:\n"
                            + qc.getLongitude()
                            + "\nAltitude:\n"
                            + qc.getAltitude()
                            );
                    data = "0|"
                            + qc.getLatitude()
                            + "|"
                            + qc.getLongitude()
                            + "|";
                    if(bterrFlag == false){
                        new Thread(){
                            public void run(){
                                try{
                                    output.write(data.getBytes());
                                    output.flush();
//                                  info="Send Successful at " + i;
                                }catch (IOException e3){
//                                  info="Send Error at " + i;
                                }
                            }
                        }.start();
                    }
                    //TODO add the code for web updates
                } else{
                    state.setText("\nGPS Garbage");
                    data = "1|0|";                                              //invalid data
         	}
                //TODO write code here to send data over bluetooth
        }
        
	public void providerStateChanged(LocationProvider provider, int newState){
	}
}