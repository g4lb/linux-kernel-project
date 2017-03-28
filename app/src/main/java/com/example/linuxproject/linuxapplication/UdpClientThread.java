package com.example.linuxproject.linuxapplication;

/**
 * Created by GalBenEvgi on 3/23/2017.
 */

import android.os.Message;
import android.util.Log;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

public class UdpClientThread extends Thread {

    String dstAddress;
    int dstPort;
    MainActivity.UdpClientHandler handler;
    DatagramSocket socket;
    InetAddress address;
    private boolean running;


    public UdpClientThread(String addr, int port, MainActivity.UdpClientHandler handler) {
        super();
        dstAddress = addr;
        dstPort = port;
        this.handler = handler;
    }

    public void setRunning(boolean running) {
        this.running = running;
    }

    private void sendState(String state) {
        handler.sendMessage(
                Message.obtain(handler,
                        MainActivity.UdpClientHandler.UPDATE_STATE, state));
    }

    @Override
    public void run() {
        sendState("connecting...");


        running = true;

        try {
            String host = "192.168.26.168";
            int port = 4545;
            String message = "getNetLink";


            // Get the internet address of the specified host
            InetAddress address = InetAddress.getByName(host);

            Log.d("D", " IP: " + address + " PORT: " + port);
            // Initialize a datagram packet with data and address
            DatagramPacket packet = new DatagramPacket(message.getBytes(), message.length(),
                    address, port);

            // Create a datagram socket, send the packet through it, close it.
            DatagramSocket dsocket = new DatagramSocket();
            dsocket.send(packet);
            //dsocket.close();

//            socket = new DatagramSocket();
//            address = InetAddress.getByName(dstAddress);

            // send request
            //byte[] buf = new byte[256];
//            byte [] buffer;
//            buffer = new String ("Send me a datagram").getBytes ();
//            byte[] data = {0x1,0x2};
//            DatagramPacket req = new DatagramPacket(data,data.length,address,dstPort);
//            DatagramSocket sock = new DatagramSocket();
//            sock.send(req);


//            Log.d("G","IP: " + address + " and PORT: " + dstPort);
//            DatagramPacket packet =
//                    new DatagramPacket(buffer, buffer.length, address, dstPort);
//            socket.send(packet);

            sendState("connected");

//            // get response
//            packet = new DatagramPacket(req, req.length);
//
//
//            socket.receive(packet);
//            String line = new String(packet.getData(), 0, packet.getLength());
//
//            handler.sendMessage(
//                    Message.obtain(handler, MainActivity.UdpClientHandler.UPDATE_MSG, packet));

        } catch (SocketException e) {
            e.printStackTrace();
        } catch (UnknownHostException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (socket != null) {
                socket.close();
                handler.sendEmptyMessage(MainActivity.UdpClientHandler.UPDATE_END);
            }
        }

    }
}
