import javax.swing.*;
import java.awt.*;
import java.io.*;
import java.util.List;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;


public class GraphVisualizer extends JPanel {
    static class Node {
        int id;
        int col;
        int row;
        int cluster;
        List<Integer> connections = new ArrayList<>();

        Node(int id, int col, int row, int cluster) {
            this.id = id;
            this.col = col;
            this.row = row;
            this.cluster = cluster;
        }
    }

    Map<Integer, Node> nodes = new HashMap<>();
    int cellSize = 50; // rozmiar jednej "kratki" w macierzy

    public GraphVisualizer(String filename) throws IOException {
        loadGraph(filename);
    }

    private void loadGraph(String filename) throws IOException {
        BufferedReader reader = new BufferedReader(new FileReader(filename));
        String line;
        while ((line = reader.readLine()) != null) {
            String[] parts = line.split(";");
            int id = Integer.parseInt(parts[0]);
            int col = Integer.parseInt(parts[1]);
            int row = Integer.parseInt(parts[2]);
            int cluster = Integer.parseInt(parts[3]);
            Node node = new Node(id, col, row, cluster);
            if (parts.length > 4) {
                String[] conn = parts[4].split(",");
                for (String c : conn) {
                    node.connections.add(Integer.parseInt(c));
                }
            }
            nodes.put(id, node);
        }
        reader.close();
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);

        Graphics2D g2 = (Graphics2D) g;
        g2.setStroke(new BasicStroke(2));

        if (nodes.isEmpty()) return;

        // Znajdź zakresy
        int minCol = Integer.MAX_VALUE, maxCol = Integer.MIN_VALUE;
        int minRow = Integer.MAX_VALUE, maxRow = Integer.MIN_VALUE;
        for (Node node : nodes.values()) {
            if (node.col < minCol) minCol = node.col;
            if (node.col > maxCol) maxCol = node.col;
            if (node.row < minRow) minRow = node.row;
            if (node.row > maxRow) maxRow = node.row;
        }

        int graphWidth = (maxCol - minCol + 1) * cellSize;
        int graphHeight = (maxRow - minRow + 1) * cellSize;

        int offsetX = (getWidth() - graphWidth) / 2 - minCol * cellSize;
        int offsetY = (getHeight() - graphHeight) / 2 - minRow * cellSize;

        // Najpierw krawędzie
        for (Node node : nodes.values()) {
            for (int connId : node.connections) {
                Node target = nodes.get(connId);
                if (target != null) {
                    g2.setColor(Color.LIGHT_GRAY);
                    g2.drawLine(
                        node.col * cellSize + offsetX,
                        node.row * cellSize + offsetY,
                        target.col * cellSize + offsetX,
                        target.row * cellSize + offsetY
                    );
                }
            }
        }

        // Teraz węzły
        for (Node node : nodes.values()) {
            Color color = getColorForCluster(node.cluster);
            g2.setColor(color);
            int x = node.col * cellSize + offsetX - 5;
            int y = node.row * cellSize + offsetY - 5;
            g2.fillOval(x, y, 10, 10);

            g2.setColor(Color.BLACK);
            g2.drawString(String.valueOf(node.id), x - 5, y - 5);
        }
    }


    private Color getColorForCluster(int cluster) {
        // Proste kolorowanie klastrów (modulo, żeby różne kolory)
        switch (cluster % 6) {
            case 0: return Color.RED;
            case 1: return Color.BLUE;
            case 2: return Color.GREEN;
            case 3: return Color.ORANGE;
            case 4: return Color.MAGENTA;
            case 5: return Color.CYAN;
            default: return Color.GRAY;
        }
    }

    public static void main(String[] args) throws IOException {
        JFrame frame = new JFrame("Graph Visualizer");
        GraphVisualizer panel = new GraphVisualizer("wynik.txt");
        frame.add(panel);
        frame.setSize(1200, 1200);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);
    }
}