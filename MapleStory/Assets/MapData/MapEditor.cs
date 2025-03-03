#if UNITY_EDITOR
using UnityEngine;
using UnityEngine.Tilemaps;
using System.IO;
using UnityEditor;

public class MapEditor
{
    [MenuItem("Tools/GenerateMap")]
    private static void GenerateMap()
    {
        bool result = EditorUtility.DisplayDialog("MapTool", "Are you sure want to generate the map data?", "Ok", "Cancel");

        if (result)
        {
            GameObject[] gameObjects = Resources.LoadAll<GameObject>("Prefabs/Map");

            foreach (GameObject go in gameObjects)
            {
                Tilemap tmBase = Util.FindChild<Tilemap>(go.transform, true, "BG");
                Tilemap tm = Util.FindChild<Tilemap>(go.transform, true, "Collision");
                if (tmBase == null || tm == null)
                {
                    Debug.Log($"{go.name} has not BG or Collision");
                    continue;
                }
                using (var writer = File.CreateText($"Assets/MapData/Data/{go.name}.txt"))
                {
                    writer.WriteLine(tmBase.cellBounds.xMin);
                    writer.WriteLine(tmBase.cellBounds.xMax);
                    writer.WriteLine(tmBase.cellBounds.yMin);
                    writer.WriteLine(tmBase.cellBounds.yMax);

                    for (int y = tmBase.cellBounds.yMax - 1; y >= tmBase.cellBounds.yMin; y--)
                    {
                        for (int x = tmBase.cellBounds.xMin; x < tmBase.cellBounds.xMax; x++)
                        {
                            TileBase tile = tm.GetTile(new Vector3Int(x, y, 0));
                            if (tile != null)
                                writer.Write("1");
                            else
                                writer.Write("0");
                        }
                        writer.WriteLine();
                    }
                }
            }

            EditorUtility.DisplayDialog("MapTool", "Map data generated successfully!", "Ok");
        }
    }
}
#endif