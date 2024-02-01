package org.example;

import java.util.*;
import org.jgrapht.Graph;
import org.jgrapht.Graphs;
import org.jgrapht.graph.DefaultWeightedEdge;
import java.util.Set;
import java.util.Vector;
import java.util.Map;
import java.util.Collections;
class MidTree{
    private Graph<String, DefaultWeightedEdge> graph;//存储从属的图
    private String root;//存储该树的跟借点
    private Vector<String> vertexSetofMidTree = new Vector<>();//存储该树存在的结点
    private Vector<DefaultWeightedEdge> edgeSetofMidTree = new Vector<>();//存储该树的根
    private Set<String> KeywordsofTree = new HashSet<>();//存储该树结点包含的类别
    private double totleweight = 0;//存储该树的权值
    MidTree(Graph<String, DefaultWeightedEdge> graph,String vertex,DefaultWeightedEdge edge,String key){
        this.graph = graph;
        this.vertexSetofMidTree.add(vertex);
        this.root = vertex;
        this.edgeSetofMidTree.add(edge);
        totleweight += graph.getEdgeWeight(edge);
        KeywordsofTree.add(key);
    }
    MidTree(Graph<String, DefaultWeightedEdge> graph,String vertex,String key){
        this.graph = graph;
        this.vertexSetofMidTree.add(vertex);
        this.root = vertex;
        this.KeywordsofTree.add(key);
    }
    MidTree(Graph<String, DefaultWeightedEdge> graph){
        this.graph = graph;
    }

    public double getTotleweight() {
        return totleweight;
    }

    public void addEdgeToMidTree(DefaultWeightedEdge e) {
        if(edgeSetofMidTree.contains(e)) return;
        edgeSetofMidTree.add(e);
        totleweight += graph.getEdgeWeight(e);
    }
    public void addVertexToMidTree(String v){
        if(vertexSetofMidTree.contains(v)) return;
        vertexSetofMidTree.add(v);
    }

    public Vector<String> getVertexSetofMidTree() {
        return vertexSetofMidTree;
    }

    public Vector<DefaultWeightedEdge> getEdgeSetofMidTree() {
        return edgeSetofMidTree;
    }

    public Set<String> getKeywordsofTree() {
        return KeywordsofTree;
    }

    public void setKeywordsofTree(String keywordsofTree) {
        KeywordsofTree.add(keywordsofTree) ;
    }
    public boolean coverKeywords(Set<String> Key){//判断树是否与传入的key完全相等
        if(KeywordsofTree.size()!= Key.size()) return false;
        for(String key : Key){
            if(!KeywordsofTree.contains(key)){
                return false;
            }
        }
        return true;
    }

    public String getRoot() {
        return root;
    }

    public void setRoot(String root) {
        this.root = root;
    }
    public boolean containKeywords(Set<String> Key){
        for(String key : Key){
            if(KeywordsofTree.contains(key)) return true;
        }
        return false;
    }

    public Graph<String, DefaultWeightedEdge> getGraph() {
        return graph;
    }
    public void printTree(){
        System.out.println("树的根结点为：" + root);
        System.out.println("树的权值为：" + totleweight);
    }
}

public class SteinerTree {
    Graph<String, DefaultWeightedEdge> graph;
    Map<String, String> PrimaryCategory;
    Vector<MidTree> Q = new Vector<>();
    Vector<MidTree> RQ = new Vector<>();
    public static void enqueue(Vector<MidTree> WQ,MidTree t){
        if(WQ.contains(t)) return;
        WQ.add(t);
    }
    public static MidTree dequeue(Vector<MidTree> WQ){
        if(WQ.size() == 0){
            return null;
        }
        MidTree now = WQ.get(0);
        WQ.remove(0);
        return  now;
    }
    public void updatequeue(Vector<MidTree> WQ){
        sortVector(WQ, Comparator.comparingDouble(MidTree::getTotleweight));
    }
    public static void sortVector(Vector<MidTree> vector, Comparator<MidTree> comparator) {
        // 使用Collections.sort对Vector进行排序，并传入Comparator
        Collections.sort(vector, comparator);
    }
    SteinerTree(Graph<String, DefaultWeightedEdge> graph,Map<String,String> PrimaryCategory){
        this.graph = graph;
        this.PrimaryCategory = PrimaryCategory;
    }
    public static Vector<MidTree> mergeTreesWithSameRoot(Vector<MidTree> Q) {
        Vector<MidTree> mergedTrees = new Vector<>();
        HashSet<Integer> processedIndices = new HashSet<>();

        for (int i = 0; i < Q.size(); i++) {
            if (processedIndices.contains(i)) continue;

            MidTree tree1 = Q.get(i);
            for (int j = i + 1; j < Q.size(); j++) {
                if (processedIndices.contains(j)) continue;

                MidTree tree2 = Q.get(j);
                if (tree1.getRoot().equals(tree2.getRoot())) {
                    // 合并 tree1 和 tree2
                    MidTree mergedTree = mergeTwoTrees(tree1, tree2);
                    mergedTrees.add(mergedTree);
                    processedIndices.add(i);
                    processedIndices.add(j);
                    break;
                }
            }

            if (!processedIndices.contains(i)) {
                mergedTrees.add(tree1);
            }
        }

        return mergedTrees;
    }

    private static MidTree mergeTwoTrees(MidTree tree1, MidTree tree2) {
        // 创建一个新的 MidTree 实例来合并 tree1 和 tree2
        MidTree mergedTree = new MidTree(tree1.getGraph());
        mergedTree.setRoot(tree1.getRoot());

        // 合并顶点集
        for (String vertex : tree1.getVertexSetofMidTree()) {
            mergedTree.addVertexToMidTree(vertex);
        }
        for (String vertex : tree2.getVertexSetofMidTree()) {
            mergedTree.addVertexToMidTree(vertex);
        }

        // 合并边集
        for (DefaultWeightedEdge edge : tree1.getEdgeSetofMidTree()) {
            mergedTree.addEdgeToMidTree(edge);
        }
        for (DefaultWeightedEdge edge : tree2.getEdgeSetofMidTree()) {
            mergedTree.addEdgeToMidTree(edge);
        }

        // 合并关键字集
        for (String keyword : tree1.getKeywordsofTree()) {
            mergedTree.setKeywordsofTree(keyword);
        }
        for (String keyword : tree2.getKeywordsofTree()) {
            mergedTree.setKeywordsofTree(keyword);
        }

        return mergedTree;
    }
    public MidTree bulidSteinerTree(Set<String> Keyword){
        Q.clear();
        RQ.clear();
        Set<String> V = graph.vertexSet();
        for(String v : V){
            if(v == null) System.out.println("v == null");
           String primarycategory = PrimaryCategory.get(v);
           if(Keyword.contains(primarycategory)){
               MidTree midTree = new MidTree(graph,v,primarycategory);
               Q.add(midTree);
           }
        }
        while(!Q.isEmpty()){
            MidTree midTree = dequeue(Q);
            if(midTree.coverKeywords(Keyword)){
                RQ.add(midTree);
                continue;
            }
            else{
                //tree growth算法
                Set<String> N = new HashSet<>();//找到所有邻居
                N.addAll(Graphs.neighborListOf(graph,midTree.getRoot()));
                Set<String> keyofmidtree = midTree.getKeywordsofTree();
                for(String n : N){//遍历邻接点
                    DefaultWeightedEdge e = graph.getEdge(n,midTree.getRoot());//对应边
                    if(e == null) continue;
                    String NprimaryCategory = PrimaryCategory.get(n);
                    Set<MidTree> Willremove = new HashSet<>();
                    Iterator<MidTree> iterator = Q.iterator();
                    Vector<MidTree> increaseQ = new Vector<>();
                    while(iterator.hasNext()){
                        MidTree q = iterator.next();
                        if(q!=midTree){
                            if(q.getRoot().equals(n)){
                                //判断两者包含的key是是否完全一致
                                keyofmidtree.add(NprimaryCategory);
                                if(q.coverKeywords(keyofmidtree)){
                                    if(q.getTotleweight()> midTree.getTotleweight()+graph.getEdgeWeight(e)){
                                        iterator.remove();
                                        midTree.setKeywordsofTree(NprimaryCategory);
                                        midTree.setRoot(n);
                                        midTree.addEdgeToMidTree(e);
                                        midTree.addVertexToMidTree(n);
                                        increaseQ.add(midTree);
                                        continue;
                                    }

                                }
                                midTree.setKeywordsofTree(NprimaryCategory);
                                midTree.setRoot(n);
                                midTree.addEdgeToMidTree(e);
                                midTree.addVertexToMidTree(n);
                                increaseQ.add(midTree);
                            }
                        }
                    }
                    for(MidTree newq : increaseQ){
                        Q.add(newq);
                    }
                    increaseQ.clear();

                }
                //treemerging//
                Q = mergeTreesWithSameRoot(Q);
                updatequeue(Q);
            }
        }
        updatequeue(RQ);
        if(RQ.isEmpty()) return null;
        return RQ.get(0);
    }

}
