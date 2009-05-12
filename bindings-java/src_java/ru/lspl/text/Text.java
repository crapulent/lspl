package ru.lspl.text;

import java.util.AbstractList;
import java.util.ArrayList;
import java.util.List;

import ru.lspl.LsplObject;
import ru.lspl.patterns.Pattern;
import ru.lspl.text.attributes.SpeechPart;

/**
 * Текст в Lspl. Текст представляется в виде графа, вершинами в котором являются некоторые точки текста, а ребрами элементы разметки текста, такие как: слова, сопоставления шаблонов и т.п.
 * @author  alno
 */
public class Text extends LsplObject {

	/**
	 * Коллекция узлов текста. Предоставляет методы для получения их количества,
	 * перебора и получения массива узлов.
	 * 
	 * @author alno
	 * 
	 */
	private class NodeList extends AbstractList<Node> {
		
		private int count = -1;

		@Override
		public int size() {
			if ( count < 0 )
				count = getNodeCount();
			
			return count;
		}

		@Override
		public Node[] toArray() {
			Node[] nodes = new Node[getNodeCount()];

			for (int i = 0; i < nodes.length; ++i)
				nodes[i] = getNode(i);

			return nodes;
		}

		@Override
		public Node get(int index) {
			return getNode( index );
		}
	}

	private class WordList extends AbstractList<Word> {

		private final int sp;
		private int count = -1;

		public WordList(int sp) {
			this.sp = sp;
		}

		@Override
		public int size() {
			if ( count < 0 )
				count = getWordCount(sp);
			
			return count;
		}

		@Override
		public Word[] toArray() {
			Word[] words = new Word[getWordCount(sp)];

			for (int i = 0; i < words.length; ++i)
				words[i] = getWord(sp, i);

			return words;
		}
		
		@Override
		public Word get(int index) {
			return getWord( sp, index );
		}
	}

	/**
	 * @author  alno
	 */
	private class MatchList extends AbstractList<Match> {

		/**
		 * @uml.property  name="pattern"
		 * @uml.associationEnd  
		 */
		private final Pattern pattern;
		
		private final int count;

		public MatchList(Pattern pattern) {
			this.pattern = pattern;
			this.count = getMatchCount(pattern);
		}

		@Override
		public int size() {
			return count;
		}

		@Override
		public Match[] toArray() {
			Match[] matches = new Match[getMatchCount(pattern)];

			for (int i = 0; i < matches.length; ++i)
				matches[i] = getMatch(pattern, i);

			return matches;
		}
		
		@Override
		public Match get(int index) {
			return getMatch( pattern, index );
		}
	}

	/**
	 * Коллекция узлов текста
	 * @uml.property  name="nodes"
	 */
	public final List<Node> nodes = new NodeList();

	/**
	 * Массив коллекций слов текста
	 * @uml.property  name="words"
	 */
	@SuppressWarnings("unchecked")
	public final List<Word>[] words = new List[13];

	/**
	 * Метод, используемый для создания нового экземпляра текста на основе
	 * строки. Новый текст содержит разметку, полученную в результате
	 * морфологического анализа.
	 * 
	 * @param source
	 *            строка, на основе которой строится текст
	 * @return новый экзмпляр текста
	 */
	public static native Text create(String source);
	
	public static native Text create(String source,TextConfig config);

	private Text(int id) {
		super(id);

		for (int i = 0; i < words.length; ++i) {
			words[i] = new WordList(i);
		}
	}
	
	/**
	 * Получить содержимое текста
	 * @return содержимое текста
	 */
	public native String getContent();

	/**
	 * Получить количество узлов в тексте
	 * 
	 * @return кол-во узлов в тексте
	 */
	public native int getNodeCount();

	/**
	 * Получить узел текста по его индексу
	 * 
	 * @param index
	 *            индекс узла
	 * @return узел текста
	 */
	public native Node getNode(int index);

	/**
	 * Получить коллекцию узлов текста
	 * @return  коллекция узлов текста
	 * @uml.property  name="nodes"
	 */
	public List<Node> getNodes() {
		return nodes;
	}

	/**
	 * Получить количество слов в тексте
	 * 
	 * @return коллекция слов в тексте
	 */
	public int getWordCount() {
		return getWordCount(0);
	}

	/**
	 * Получить слово в тексте по его индексу
	 * 
	 * @param index
	 *            индекс слова
	 * @return слово текста
	 */
	public Word getWord(int index) {
		return getWord(0, index);
	}

	/**
	 * Получить коллекцию слов текста
	 * @return  коллекция слов текста
	 * @uml.property  name="words"
	 */
	public List<Word> getWords() {
		return words[0];
	}

	/**
	 * Получить количество слов текста заданной части речи
	 * 
	 * @param speechPart
	 *            индекс части речи
	 * @return количество слов
	 */
	public native int getWordCount(int speechPart);

	/**
	 * Получить слово текста заданной части речи по индексу
	 * 
	 * @param speechPart
	 *            индекс части речи
	 * @param index
	 *            индекс слова
	 * @return слово
	 */
	public native Word getWord(int speechPart, int index);

	/**
	 * Получить коллекцию слов текста заданной части речи
	 * 
	 * @param speechPart
	 *            индекс части речи
	 * @return коллекция слов
	 */
	public List<Word> getWords(int speechPart) {
		return words[speechPart];
	}

	/**
	 * Получить количество слов текста заданной части речи
	 * 
	 * @param speechPart
	 *            часть речи
	 * @return количество слов
	 */
	public int getWordCount(SpeechPart speechPart) {
		return getWordCount(speechPart.ordinal());
	}

	/**
	 * Получить слово текста заданной части речи по индексу
	 * 
	 * @param speechPart
	 *            часть речи
	 * @param index
	 *            индекс слова
	 * @return слово
	 */
	public Word getWord(SpeechPart speechPart, int index) {
		return getWord(speechPart.ordinal());
	}

	/**
	 * Получить коллекцию слов текста заданной части речи
	 * 
	 * @param speechPart
	 *            часть речи
	 * @return коллекция слов
	 */
	public List<Word> getWords(SpeechPart speechPart) {
		return words[speechPart.ordinal()];
	}
	
	public List<Transition> getTransitions() {
		ArrayList<Transition> transitions = new ArrayList<Transition>();
		
		for (Node n : nodes)
			transitions.addAll( n.getTransitions() );
		
		return transitions;
	}
	
	public Transition[] getTransitionsArray() {
		int size = 0;

		for (Node n : nodes) // Подсчитываем кол-во переходов в тексте
			size += n.getTransitionCount();

		Transition[] transitions = new Transition[size];
		int i = 0;

		for (Node n : nodes) // Заполняем массив переходов
			for (Transition t : n.transitions)
				transitions[i++] = t;

		return transitions;
	}

	public native int getMatchCount(Pattern pattern);

	public native Match getMatch(Pattern pattern, int index);

	public List<Match> getMatches(Pattern pattern) {
		return new MatchList(pattern);
	}

	public List<Match> getMatches(Iterable<Pattern> patterns) {
		ArrayList<Match> matches = new ArrayList<Match>();

		for (Pattern pattern : patterns) {
			int count = getMatchCount(pattern);

			for (int i = 0; i < count; ++i)
				matches.add(getMatch(pattern, i));
		}

		return matches;
	}

	public Pattern[] selectMatched(Pattern[] patterns) {
		ArrayList<Pattern> matched = new ArrayList<Pattern>();

		for (Pattern pattern : patterns)
			if (getMatchCount(pattern) > 0)
				matched.add(pattern);

		return matched.toArray(new Pattern[matched.size()]);
	}

	/**
	 * Проверить, имеет ли хотя бы один шаблон из массива сопоставление в тексте
	 * 
	 * @param patterns
	 *            массив шаблонов
	 * @return true, если есть шаблон из массива, который имеет сопоставление
	 */
	public boolean hasAnyMatches(Pattern... patterns) {
		for (Pattern pattern : patterns)
			if (getMatchCount(pattern) > 0)
				return true;

		return false;
	}

	/**
	 * Проверить, имеет ли хотя бы один шаблон из коллекции сопоставление в
	 * тексте
	 * 
	 * @param patterns
	 *            коллекция шаблонов
	 * @return true, если есть шаблон из коллекции, который имеет сопоставление
	 */
	public boolean hasAnyMatches(Iterable<? extends Pattern> patterns) {
		for (Pattern pattern : patterns)
			if (getMatchCount(pattern) > 0)
				return true;

		return false;
	}

	/**
	 * Проверить, имеют ли все шаблоны из массива сопоставления в тексте
	 * 
	 * @param patterns
	 *            массив шаблонов
	 * @return true, если все шаблоны имеют сопоставления в тексте
	 */
	public boolean hasAllMatches(Pattern... patterns) {
		for (Pattern pattern : patterns)
			if (getMatchCount(pattern) <= 0)
				return false;

		return true;
	}
	
	/**
	 * Проверить, имеют ли все шаблоны из коллекции сопоставления в тексте
	 * 
	 * @param patterns
	 *            коллекция шаблонов
	 * @return true, если все шаблоны имеют сопоставления в тексте
	 */
	public boolean hasAllMatches(Iterable<? extends Pattern> patterns) {
		for (Pattern pattern : patterns)
			if (getMatchCount(pattern) <= 0)
				return false;

		return true;
	}

	/**
	 * Сбросить отладочное представление структуры текста в строку
	 * 
	 * @return отладочное представление текста
	 */
	public native String dump();
	
	protected native void finalize();
}