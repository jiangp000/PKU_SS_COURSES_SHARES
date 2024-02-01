package org.example;

import org.apache.commons.csv.CSVFormat;
import org.apache.commons.csv.CSVParser;
import org.apache.commons.csv.CSVRecord;
import org.jgrapht.Graph;
import org.jgrapht.graph.DefaultWeightedEdge;
import org.jgrapht.graph.SimpleWeightedGraph;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;


public class ApiGraphBuilder {

    //用于保存顶点的属性
    public static Map<String, String> PrimaryCategory = new HashMap<>();
    // 添加带权值的边到带权图中
    private static DefaultWeightedEdge addWeightedEdge(Graph<String, DefaultWeightedEdge> graph, String source, String target, double weight) {
        DefaultWeightedEdge edge = graph.addEdge(source, target);
        graph.setEdgeWeight(edge, weight);
        return edge;
    }

    // 修改边的权值
    private static void setEdgeWeight(Graph<String, DefaultWeightedEdge> graph, DefaultWeightedEdge edge, double weight) {
        graph.setEdgeWeight(edge, weight);
    }

    private static DefaultWeightedEdge findEdge(Graph<String, DefaultWeightedEdge> graph, String source, String target) {
        return graph.getEdge(source, target);
    }
    private static void printWeightedGraph(Graph<String, DefaultWeightedEdge> graph) {
        System.out.println("Vertices:");
        for (String vertex : graph.vertexSet()) {
            System.out.print(vertex + " ");
        }
        System.out.println();

        System.out.println("Edges with weights:");
        for (DefaultWeightedEdge edge : graph.edgeSet()) {
            String source = graph.getEdgeSource(edge);
            String target = graph.getEdgeTarget(edge);
            double weight = graph.getEdgeWeight(edge);
            System.out.println(source + " -- " + target + " (weight: " + weight + ")");
        }
    }
    public static void main(String[] args) {
        //定义一个无向图
        Graph<String, DefaultWeightedEdge> undirectedGraph = new SimpleWeightedGraph<>(DefaultWeightedEdge.class);
        // 读取 api.csv 文件，构建图数据结构
        String apiCsvFile = "api.csv";

        try {
            CSVParser csvParser = new CSVParser(new FileReader(apiCsvFile), CSVFormat.DEFAULT.withHeader());
            for (CSVRecord record : csvParser) {
                String name = record.get("Name");
                String category = record.get("Primary Category");
                undirectedGraph.addVertex(name);
                PrimaryCategory.put(name,category);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        // 读取 mashup.csv 文件，计算边的权值
        String mashupCsvFile = "mashup.csv";

        try {
            CSVParser csvParser = new CSVParser(new FileReader(mashupCsvFile), CSVFormat.DEFAULT.withHeader());
            for (CSVRecord record : csvParser) {
                String[] relatedApis = record.get("Related APIs").split(",");
                for(String a :relatedApis){
                    for(String b :relatedApis){
                        if(undirectedGraph.containsVertex(a)&&undirectedGraph.containsVertex(b)){
                            if(a != b){
                                if(!undirectedGraph.containsEdge(a,b)){
                                    addWeightedEdge(undirectedGraph,a,b,1);
                                }
                                else{
                                    DefaultWeightedEdge targetEdge = findEdge(undirectedGraph,a,b);
                                    setEdgeWeight(undirectedGraph,targetEdge, undirectedGraph.getEdgeWeight(targetEdge) + 1);
                                }
                            }
                        }

                    }
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        printWeightedGraph(undirectedGraph);
        ////////////////////////////
        //以下是算法验证部分
        ///////////////////////////
        for (DefaultWeightedEdge edge : undirectedGraph.edgeSet()) {//首先根据算法要求将权值取倒数
            undirectedGraph.setEdgeWeight(edge,1/undirectedGraph.getEdgeWeight(edge));
        }
        // 生成群斯坦纳树
        SteinerTree steinerTree = new SteinerTree(undirectedGraph,PrimaryCategory);
        Set<String> keywords = new HashSet<>();
        keywords.add("Software-as-a-Service");
        keywords.add("ERP");
        MidTree res =steinerTree.bulidSteinerTree(keywords);
        // 生成群斯坦纳树
        if(res != null){
            Vector<String> V= res.getVertexSetofMidTree();
            for(String v :V){
                System.out.println(v);
            }
        }
        else{
            System.out.println("null");
        }
        try {
            CSVParser csvParser = new CSVParser(new FileReader(mashupCsvFile), CSVFormat.DEFAULT.withHeader());
            AtomicInteger totle = new AtomicInteger(0);
            AtomicInteger right = new AtomicInteger(0);
            AtomicInteger lessthan = new AtomicInteger(0);
            //被注释掉的是单线程部分，没有完善输出，只能显示进度和命中的次数
            /*double totle = 0;//数据总条数，用来显示算法进度
            double right = 0;//命中条数
            Set<CSVRecord> csvSet = new HashSet<>(csvParser.getRecords());
            for (CSVRecord record : csvSet) {
                totle++;
                System.out.println(totle);

                String[] relatedApis = record.get("Related APIs").split(",");
                String[] Categories = record.get("Categories").split(",");
                Set<String> comparedres = new HashSet<>();
                Set<String> Query = new HashSet<>();

                for(String a :relatedApis){
                    comparedres.add(a);
                }
                for(String b : Categories){
                    Query.add(b);
                }
                MidTree ress =steinerTree.bulidSteinerTree(Query);
                if(ress != null){
                    Vector<String> resV = ress.getVertexSetofMidTree();
                    if(comparedres.containsAll(resV)) {
                        System.out.println("第“+totle+”条命中了");
                        right++;
                    }
                }

            }*/

            Set<CSVRecord> csvSet = new HashSet<>(csvParser.getRecords());
            ExecutorService executor = Executors.newFixedThreadPool(Runtime.getRuntime().availableProcessors());
            System.out.println("一共" + csvSet.size()+"任务,任务开始");
            for (CSVRecord record : csvSet) {
                executor.submit(() -> {
                    totle.incrementAndGet();
                    System.out.println("一共" + csvSet.size()+"任务,这是第"+totle.get()+"个任务");

                    String[] relatedApis = record.get("Related APIs").split(",");
                    String[] Categories = record.get("Categories").split(",");
                    Set<String> comparedres = new HashSet<>();
                    Set<String> Query = new HashSet<>();

                    for (String a : relatedApis) {
                        comparedres.add(a);
                    }
                    for (String b : Categories) {
                        Query.add(b);
                    }
                    SteinerTree steinerTree1 = new SteinerTree(undirectedGraph,PrimaryCategory);
                    long startTime = System.currentTimeMillis();
                    MidTree ress = steinerTree1.bulidSteinerTree(Query);
                    long endTime = System.currentTimeMillis();
                    if (ress != null) {
                        System.out.println("第"+totle.get()+"个任务返回api共耗时"+ (endTime - startTime)+"毫秒");
                        System.out.println("第"+totle.get()+"个任务返回的群Steiner Tree的权重和为："+ ress.getTotleweight());
                        Vector<String> resV = ress.getVertexSetofMidTree();
                        System.out.println("第"+totle.get()+"个任务返回的群Steiner Tree的顶点个数为:"+ resV.size());
                        if(resV.size()<=comparedres.size()) lessthan.incrementAndGet();
                        if (comparedres.containsAll(resV)) {
                            right.incrementAndGet();
                            System.out.println("第"+totle.get()+"个任务命中"+"这是第" + right.get()+"个命中");
                        }
                        else{
                            System.out.println("第"+totle.get()+"个任务未命中");
                        }
                    }
                    else{
                        System.out.println("第"+totle.get()+"个任务未命中");
                    }
                });
            }

            executor.shutdown();
            try {
                executor.awaitTermination(Long.MAX_VALUE, TimeUnit.NANOSECONDS);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            double hitrate = right.doubleValue()/totle.doubleValue();
            System.out.println("一共命中了"+right.get()+"个任务，"+"命中率为" + hitrate*100+"%");
            System.out.println("若返回的api数少于所给的api数也算命中，则命中率为"+(lessthan.doubleValue()/totle.doubleValue())*100 + "%");
            System.out.println("任务结束");
        } catch (IOException e) {
            e.printStackTrace();
        }

    }





}
