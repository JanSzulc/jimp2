import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.Group;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Line;
import javafx.stage.Stage;
import java.io.*;
import java.util.*;

public class GraphVisualizer extends Application {

    static Map<Integer, Integer> nodePart = new HashMap<>();
    static List<int[]> edges = new ArrayList<>();

    @Override
    public void start(Stage stage) {
        Group root = new Group();

        int radius = 200;
        int centerX = 300;
        int centerY = 300;
        int n = nodePart.size();

        Map<Integer, Circle> nodeCircles = new HashMap<>();

        for (int i = 0; i < n; i++) {
            double angle = 2 * Math.PI * i / n;
            double x = centerX + radius * Math.cos(angle);
            double y = centerY + radius * Math.sin(angle);

            Color color;
            switch (nodePart.get(i)) {
                case 0:
                    color = Color.RED;
                    break;
                case 1:
                    color = Color.GREEN;
                    break;
                case 2:
                    color = Color.BLUE;
                    break;
                default:
                    color = Color.BLACK;
                    break;
            }

            Circle circle = new Circle(x, y, 10, color);
            nodeCircles.put(i, circle);
            root.getChildren().add(circle);
        }

        // Teraz rysujemy krawędzie
        for (int[] edge : edges) {
            Circle c1 = nodeCircles.get(edge[0]);
            Circle c2 = nodeCircles.get(edge[1]);
            Line line = new Line(c1.getCenterX(), c1.getCenterY(), c2.getCenterX(), c2.getCenterY());
            line.setStroke(Color.GRAY);
            root.getChildren().add(line);
        }

        Scene scene = new Scene(root, 600, 600);
        stage.setTitle("Graph Visualization (structured wynik.txt)");
        stage.setScene(scene);
        stage.show();
    }

    public static void main(String[] args) {
        try (BufferedReader br = new BufferedReader(new FileReader("wynik.txt"))) {
            String line;
            boolean readingNodes = true;
            while ((line = br.readLine()) != null) {
                line = line.trim();
                if (line.isEmpty()) continue;
                if (line.startsWith("#")) continue; // komentarze

                String[] parts = line.split("\\s+");
                if (parts.length == 2) {
                    int a = Integer.parseInt(parts[0]);
                    int b = Integer.parseInt(parts[1]);
                    if (readingNodes) {
                        nodePart.put(a, b);
                    } else {
                        edges.add(new int[]{a, b});
                    }
                } else {
                    readingNodes = false; // po pustej linii przechodzimy do krawędzi
                }
            }
        } catch (IOException e) {
            System.err.println("Błąd podczas czytania wynik.txt: " + e.getMessage());
            return;
        }

        launch();
    }
}
