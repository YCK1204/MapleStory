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

                int minX = int.MaxValue, minY = int.MaxValue;
                int maxX = int.MinValue, maxY = int.MinValue;

                BoundsInt bounds = tmBase.cellBounds;

                foreach (var pos in bounds.allPositionsWithin)
                {
                    if (tmBase.GetTile(pos) != null) // 실제 타일이 있는 경우만 체크
                    {
                        if (pos.x < minX) minX = pos.x;
                        if (pos.y < minY) minY = pos.y;
                        if (pos.x > maxX) maxX = pos.x;
                        if (pos.y > maxY) maxY = pos.y;
                    }
                }

                using (var writer = File.CreateText($"Assets/MapData/Data/{go.name}.txt"))
                {
                    writer.WriteLine(minX);
                    writer.WriteLine(maxX);
                    writer.WriteLine(minY);
                    writer.WriteLine(maxY);

                    for (int y = maxY; y >= minY; y--)
                    {
                        for (int x = minX; x <= maxX; x++)
                        {
                            TileBase tile = tm.GetTile(new Vector3Int(x, y, 0));
                            writer.Write(tile != null ? "1" : "0");
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