import java.io.*;
import java.net.*;

public class HttpMirror {
    public static void main(String[] args) {
        try {
            int port = Integer.parseInt(args[0]);
            ServerSocket ss = new ServerSocket(port);
            for (;;) {
                Socket client = ss.accept();
                BufferedReader in = new BufferedReader(
                        new InputStreamReader(client.getInputStream()));
                PrintWriter out = new PrintWriter(client.getOutputStream());

                out.print("HTTP/1.0 200 \n");
                out.print("Content-Type: text/plain\n");
                out.print("\n");

                String line;
                while ((line = in.readLine()) != null) {
                    if (line.length() == 0) break;
                    out.print(line + "\n");
                    System.out.println(line);
                }

                out.close();
                in.close();
                client.close();
            }
        } catch (Exception e) {
            System.err.println(e);
            System.err.println("Usage: java HTTPMirror <port>");
            System.err.println("    e.g.: java HttpMirror 4444");
        }
    }
}
