import java.util.*;

public class Solution {
    public List<List<String>> findLadders(String beginWord, String endWord, Set<String> wordList) {
        wordList.add(endWord);
        
        Map<String, List<String>> prefix = new HashMap<>();
        for(String word : wordList) {
            prefix.put(word, new ArrayList<String>());
        } 

        Set<String> done = new HashSet<>();
        done.add(beginWord);

        while(!done.isEmpty()) {
            Set<String> tbd =  new HashSet<>();
            for(String ds : done) {
                for(String word : wordList) {
                    int diff = 0;
                    for(int i = 0; i < word.length(); ++ i) {
                        if(ds.charAt(i) != word.charAt(i)) {
                            ++ diff;
                            if(diff > 1) {
                                break;
                            }
                        }
                    }
                    if(diff == 1) {
                        tbd.add(word);
                        prefix.get(word).add(ds);
                    }
                }
            }

            wordList.removeAll(tbd);

            if(tbd.contains(endWord)) {
                break;
            }

            done = tbd;
        }
        
        // build result
        List<List<String>> result = new ArrayList<>();
        ArrayDeque<String> path = new ArrayDeque<>();
        
        buildResult(result, path, beginWord, endWord, prefix);
        
        return result;
    }
    
    private void buildResult(List<List<String>> result, ArrayDeque<String> path, String beginWord, String endWord, Map<String, List<String>> prefix) {
        path.addFirst(endWord);
        if(beginWord == endWord) {
            result.add(new ArrayList<String>(path));
        }
        else {
            for(String pre : prefix.get(endWord)) {
                buildResult(result, path, beginWord, pre, prefix);
            }
        }
        path.removeFirst();
    }

    public static void main(String[] args) {
        String beginWord = "hit";
        String endWord = "cog";
        HashSet<String> wordList = new HashSet<>(Arrays.asList("hot","dot","dog","lot","log"));

        System.out.println(new Solution().findLadders(beginWord, endWord, wordList));
    }
}
